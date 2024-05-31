#include "game.hxx"

void Game::init() {
    Log::init();

    events_ = std::make_unique<EventManager>();
    config_manager_ = std::make_unique<ConfigManager>(events_.get());
    services_ = std::make_unique<ServiceLocator>();
    input_ = std::make_unique<Input>(events_.get());
    platform_ = std::make_unique<Platform>(events_.get(), input_.get());
    platform_->initialize();

    world_ = std::make_unique<World>();
    entity_manager_ = std::make_unique<EntityManager>();
    tile_manager_ = std::make_unique<TileManager>();
    action_queue_ = std::make_unique<ActionQueue>(events_.get(), services_.get());
    catalog_ = std::make_unique<Catalog>();

    services_->provide<ConfigManager>(config_manager_.get());
    services_->provide<EventManager>(events_.get());
    services_->provide<World>(world_.get());
    services_->provide<IEntityReader>(entity_manager_.get());
    services_->provide<IEntityWriter>(entity_manager_.get());
    services_->provide<EntityManager>(entity_manager_.get());
    services_->provide<TileManager>(tile_manager_.get());
    services_->provide<ITileReader>(tile_manager_.get());
    services_->provide<ITileWriter>(tile_manager_.get());
    services_->provide<Catalog>(catalog_.get());
    services_->provide<ActionQueue>(action_queue_.get());
    services_->provide<IActionCreator>(action_queue_.get());
    services_->provide<IActionProcessor>(action_queue_.get());
    services_->provide<IInputReader>(input_.get());

    Renderer::init(events_.get());
    Renderer::set_viewport(platform_->window_size());

    Ui::init(events_.get(), &Renderer::ui_layer());
    Scripting::init(events_.get());

    Translator::load("en");

    // xray / engine ui
    Xray::init(events_.get());
    Xray::add<LogXray>();
    Xray::add<SceneXray>(entity_manager_.get(), tile_manager_.get(), events_.get(), input_.get());
    Xray::add<ScriptXray>(events_.get());
    Xray::add<UiXray>();

    // scripting
    Scripting::add_api<LogApi>();
    Scripting::add_api<ConfigApi>(config_manager_.get(), events_.get());
    Scripting::add_api<SceneApi>(entity_manager_.get());
    Scripting::add_api<UiApi>();
    Scripting::add_api<CatalogApi>(catalog_.get(), services_.get());

    Scripting::load_from_path(Scripting::default_script_path);

    // post initialization experimentation
    {
        auto arch_troll = catalog_->archetype("TROLL");
        auto arch_dwarf = catalog_->archetype("DWARF");
        if (arch_troll) {
            auto& troll = entity_manager_->create_entity(*arch_troll);
            troll.position = { 3, 3 };
        } else {
            Log::warn("TROLL archetype not yet present");
        }
        if (arch_dwarf) {
            auto& dwarf = entity_manager_->create_entity(*arch_dwarf);
            dwarf.position = { 0, 1 };

            entity_manager_->set_controlled_entity(&dwarf);
        } else {
            Log::warn("DWARF archetype not yet present");
        }
    }
}

void Game::shutdown() {
    Xray::shutdown();
    Scripting::shutdown();
    Ui::shutdown();
    Renderer::shutdown();
    platform_->shutdown();
}

void Game::run() {
    init();

    while (true) {
        if (!platform_->prepare()) {
            break;
        }

        Ui::update();

        // TODO: temporary code
        auto& world_layer = Renderer::display();
        const auto& last_tile_index { world_layer.cell_count() };
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
                
                if (!tile->revealed) {
                    continue;
                }

                auto cell = tile->display_info;

                // logic does not work with mutliple layers (game + ui)
                // the ui will be "burned in"
                // TODO: draw world and ui into separate buffers and then merge them

                if (!tile->fov) {
                    world_layer.at(tile_pos)->color = Color::mono(128);
                } else {
                    world_layer.put(std::move(cell), tile_pos);
                }
            }
        }

        for (const auto& entity : entity_manager_->entities()) {
            if (!world_layer.in_bounds(entity->position)) {
                continue;
            }

            Render* render_component = entity->component<Render>();
            if (!render_component) {
                continue;
            }
            const auto& info = render_component->display_info();
            if (!info.visible) {
                continue;
            }

            const auto tile { tiles.at(entity->position) };
            if (!tile || !tile->fov) {
                continue;
            }
            world_layer.put(DisplayCell(info.glyph, info.color), entity->position);
        }

        Ui::draw();
        Renderer::render();
        Xray::draw();

        platform_->present();
    }

    shutdown();
}
