#include "game.hxx"

void Game::init() {
    Log::init();

    game_events_ = std::make_unique<EventManager<GameEventType>>();
    engine_events_ = std::make_unique<EventManager<EngineEventType>>();
    events_ = std::make_unique<Events>(game_events_.get(), engine_events_.get());
    config_manager_ = std::make_unique<ConfigManager>(events_.get());
    services_ = std::make_unique<ServiceLocator>();
    input_ = std::make_unique<Input>(events_.get());
    scripting_ = std::make_unique<Scripting>(events_.get());
    platform_ = std::make_unique<Platform>(events_.get(), input_.get());
    platform_->initialize();

    {
        auto path = fs::absolute(fmt::format("dictionaries/en.toml")).string();
        auto result = TranslationLoader().load(path);
        if (result.error != LoadTranslationError::None) {
            Log::error("Failed to load translation file: err #{}", (int) result.error);
        }
        t_ = std::make_unique<Translator>(std::move(result.dictionary));
    }
    services_->provide<Translator>(t_.get());
    entity_manager_ = std::make_unique<EntityManager>(events_.get());
    tile_manager_ = std::make_unique<TileManager>();

    chunk_generator_ = std::make_unique<ChunkGenerator>();
    chunk_manager_ = std::make_unique<ChunkManager>(chunk_generator_.get(), events_.get());
    tile_accessor_ = std::make_unique<TileAccessor>(chunk_manager_.get());

    action_queue_ = std::make_unique<ActionQueue>(events_.get(), services_.get());
    world_ = std::make_unique<World>(entity_manager_.get(), action_queue_.get(), events_.get());
    vision_manager_ = std::make_unique<VisionManager>(entity_manager_.get(), tile_manager_.get(), events_.get());
    catalog_ = std::make_unique<Catalog>();

    services_->provide<VisionManager>(vision_manager_.get());
    services_->provide<ConfigManager>(config_manager_.get());
    services_->provide<Events>(events_.get());
    services_->provide<EventManager<GameEventType>>(events_->game);
    services_->provide<EventManager<EngineEventType>>(events_->engine);
    services_->provide<Events>(events_.get());
    services_->provide<World>(world_.get());
    services_->provide<IEntityReader>(entity_manager_.get());
    services_->provide<IEntityWriter>(entity_manager_.get());
    services_->provide<EntityManager>(entity_manager_.get());
    services_->provide<TileManager>(tile_manager_.get());
    services_->provide<TileAccessor>(tile_accessor_.get());
    services_->provide<ITileReader>(tile_manager_.get());
    services_->provide<ITileWriter>(tile_manager_.get());
    services_->provide<Catalog>(catalog_.get());
    services_->provide<ActionQueue>(action_queue_.get());
    services_->provide<IActionCreator>(action_queue_.get());
    services_->provide<IActionProcessor>(action_queue_.get());
    services_->provide<IInputReader>(input_.get());

    player_controller_ = std::make_unique<GamePlayerController>(
        entity_manager_.get(),
        action_queue_.get(),
        events_.get()
    );
    world_->bind_player_controller(player_controller_.get());

    WorldSpecCreator world_spec_creator;

    world_spec_ = world_spec_creator.create_world_spec(CreateWorldSpecOptions {
        .seed = 0,
        .chunks_x = 256,
        .chunks_z = 256,
        .max_vegetation = static_cast<i32>(0.8f * Chunk::CHUNK_DEPTH),
        .max_shoreline = static_cast<i32>(0.45f * Chunk::CHUNK_DEPTH),
        .max_water = static_cast<i32>(0.4f * Chunk::CHUNK_DEPTH)
    });

    Renderer::init(events_.get());
    Renderer::set_viewport(platform_->window_size());

    Ui::init(events_.get(), &Renderer::ui_layer());

    // xray / engine ui
    Xray::init(events_.get());
    Xray::add<LogXray>();
    Xray::add<SceneXray>(world_spec_.get(), chunk_manager_.get(), entity_manager_.get(), tile_accessor_.get(), tile_manager_.get(), events_.get(), input_.get(), t_.get());
    Xray::add<ScriptXray>(scripting_.get(), events_.get());
    Xray::add<UiXray>();
    Xray::add<PerfXray>();

    // scripting
    scripting_->add_api<LogApi>();
    scripting_->add_api<ConfigApi>(config_manager_.get(), events_.get());
    scripting_->add_api<SceneApi>(entity_manager_.get());
    scripting_->add_api<UiApi>();
    scripting_->add_api<CatalogApi>(catalog_.get(), services_.get());
    scripting_->reload();

    events_->engine->trigger<WorldReadyEvent>(world_spec_.get());

    // post initialization experimentation
    {
        auto arch_troll = catalog_->archetype("TROLL");
        auto arch_human = catalog_->archetype("HUMAN");
        if (arch_troll) {
            entity_manager_->create_entity(*arch_troll, WorldPos(3, 0, 3));
        } else {
            Log::warn("TROLL archetype not yet present");
        }
        if (arch_human) {
            auto& human = entity_manager_->create_entity(*arch_human, WorldPos(1, 0, 1));
            human.position.y = chunk_manager_->get_chunk(human.position)->height_map[human.position.x + human.position.z * Chunk::CHUNK_SIDE_LENGTH];
            entity_manager_->set_controlled_entity(&human);
            world_->get_camera_controller().set_target(&human);
        } else {
            Log::warn("HUMAN archetype not yet present");
        }
    }
}

void Game::shutdown() {
    Xray::shutdown();
    Ui::shutdown();
    Renderer::shutdown();

    catalog_.reset();
    vision_manager_.reset();
    world_.reset();
    action_queue_.reset();
    tile_accessor_.reset();
    chunk_manager_.reset();
    chunk_generator_.reset();
    tile_manager_.reset();
    entity_manager_.reset();
    player_controller_.reset();

    scripting_.reset();
    services_.reset();
    input_.reset();
    config_manager_.reset();
    events_.reset();
    game_events_.reset();
    engine_events_.reset();

    platform_->shutdown();
}

Game::~Game() {
    shutdown();
}

int Game::start() {
    Game game;
    game.run();
    return 0;
}

void Game::run() {
    init();

    while (true) {
        Profiler::timer("Frame").start();

        if (!platform_->prepare()) {
            break;
        }
        Ui::update();

        // TODO: temporary code
        Profiler::timer("Draw").start();
        auto& world_layer = Renderer::display();

        world_layer.reset();

        const auto camera_pos = world_->get_camera().position;
        i32 display_half_width = world_layer.dimensions().x / 2;
        i32 display_half_height = world_layer.dimensions().y / 2;

        const i32 left_bound = camera_pos.x - display_half_width;
        const i32 right_bound = camera_pos.x + display_half_width;
        const i32 top_bound = camera_pos.z - display_half_height;
        const i32 bottom_bound = camera_pos.z + display_half_height;

        for (i32 z = top_bound; z <= bottom_bound; ++z) {
            for (i32 x = left_bound; x <= right_bound; ++x) {
                Tile* tile = tile_accessor_->get_tile(WorldPos(x, camera_pos.y, z));
                if (tile) {
                    u32 screen_x = x - left_bound;
                    u32 screen_z = z - top_bound;

                    if (!tile->flags.test(TileFlags::Revealed)) {
                        continue;
                    }
                    world_layer.put(tile->display_info, Vec2<u32>(screen_x, screen_z));
                }
            }
        }

        /*
        Grid<Tile>& tiles { tile_manager_->tiles() };
        const auto scene_dim { tiles.dimensions() };
        const auto display_dim { world_layer.dimensions() };
        const Vec2<u32> tile_render_dim (
            std::min<u32>(scene_dim.x, display_dim.x),
            std::min<u32>(scene_dim.y, display_dim.y)
        );
        for (u32 y = 0; y < tile_render_dim.y; ++y) {
            for (u32 x = 0; x < tile_render_dim.x; ++x) {
                const Vec2<u32> tile_pos { x, y };
                const Tile* tile { tiles.at(tile_pos) };

                auto cell = tile->display_info;

                // logic does not work with mutliple layers (game + ui)
                // the ui will be "burned in"
                // TODO: draw world and ui into separate buffers and then merge them

                if (tile->flags.test(TileFlags::FoV)) {
                    world_layer.put(std::move(cell), tile_pos);
                } else {
                    world_layer.at(tile_pos)->color = Color::mono(128);
                }
            }
        }
        */
        for (const auto& entity : entity_manager_->entities()) {
            auto pos = entity->position - WorldPos(left_bound, 0, top_bound);

            if (!world_layer.in_bounds(Vec2<u32>(pos.x, pos.z))) {
                continue;
            }

            auto render_component = entity->component<Render>();
            if (!render_component) {
                continue;
            }
            const auto& info = render_component->display_info();
            if (!info.visible) {
                continue;
            }

            //const auto tile { tiles.at(entity->position) };
            //if (!tile || !tile->flags.test(TileFlags::FoV)) {
            //    continue;
            //}

            if (entity->position.y != camera_pos.y) {
                continue;
            }

            world_layer.put(DisplayCell(info.glyph, info.color), Vec2<u32>(pos.x, pos.z));
        }
        Ui::draw();
        Profiler::timer("Draw").stop();
        Profiler::timer("Render").start();
        Renderer::render();
        Profiler::timer("Render").stop();

        Xray::draw();

        platform_->present();
        Profiler::timer("Frame").stop();
    }

    shutdown();
}
