#include "game.hxx"

void Game::init() {
    Log::init();

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Log::error("SDL could not initialize: {}", SDL_GetError());
        std::abort();
    }

    platform_ = std::make_unique<Platform>(create_platform_api());
    platform_->create_window("Jaralyn", { 1600, 900 });

    services_ = std::make_unique<ServiceLocator>();
    events_ = std::make_unique<EventManager>();
    world_ = std::make_unique<World>();
    entity_manager_ = std::make_unique<EntityManager>();
    tile_manager_ = std::make_unique<TileManager>();
    action_queue_ = std::make_unique<ActionQueue>(events_.get(), services_.get());
    catalog_ = std::make_unique<Catalog>();

    services_->provide(events_.get());
    services_->provide(world_.get());
    services_->provide<IEntityReader>(entity_manager_.get());
    services_->provide<IEntityWriter>(entity_manager_.get());
    services_->provide<ITileReader>(tile_manager_.get());
    services_->provide<Catalog>(catalog_.get());

    services_->provide(action_queue_.get());
    services_->provide<IActionCreator>(action_queue_.get());
    services_->provide<IActionProcessor>(action_queue_.get());

    events_->on<ScriptLoadedEvent>(this, &Game::on_script_loaded);

    Renderer::init(platform_.get(), events_.get());
    Ui::init(events_.get(), &Renderer::ui_layer());
    Scripting::init(events_.get());

    Translator::load("en");

    // xray / engine ui
    Xray::init(events_.get(), platform_->sdl_window(), Renderer::gl_context());
    Xray::add<LogXray>();
    Xray::add<SceneXray>(entity_manager_.get(), tile_manager_.get(), events_.get());
    Xray::add<ScriptXray>(events_.get());
    Xray::add<UiXray>();

    // scripting
    Scripting::add_api<LogApi>();
    Scripting::add_api<SceneApi>(entity_manager_.get());
    Scripting::add_api<UiApi>();
    Scripting::add_api<CatalogApi>(catalog_.get(), services_.get());

    Scripting::load_from_path(Scripting::default_script_path);

    is_running_ = true;

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

void Game::process_os_messages() {
    // TODO: to platform
    SDL_PumpEvents();

    SDL_Event e;
    while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_EventType::SDL_FIRSTEVENT, SDL_EventType::SDL_MOUSEWHEEL) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&e);

        switch (e.type) {
            case SDL_EventType::SDL_QUIT:
                is_running_ = false;
                return;
            case SDL_EventType::SDL_WINDOWEVENT: {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    events_->trigger<ResizeEvent>(Vec2<i32> {
                        static_cast<i32> (e.window.data1),
                        static_cast<i32> (e.window.data2)
                    });
                }
                break;
            }
            case SDL_EventType::SDL_KEYDOWN: {
                const Key key { static_cast<Key>(e.key.keysym.sym) };
                Input::keyboard_.key_down(key);
                events_->trigger<KeyDownEvent>(key);
                break;
            }
            case SDL_EventType::SDL_KEYUP: {
                const auto key { static_cast<Key>(e.key.keysym.sym) };
                Input::keyboard_.key_up(key);
                events_->trigger<KeyUpEvent>(key);
                break;
            }
            case SDL_EventType::SDL_MOUSEBUTTONDOWN: {
                const auto button { static_cast<MouseButton>(e.button.button) };
                Input::mouse_.mouse_down(button);
                events_->trigger<MouseDownEvent>(button);
                break;
            }
            case SDL_EventType::SDL_MOUSEBUTTONUP: {
                const auto button { static_cast<MouseButton>(e.button.button) };
                Input::mouse_.mouse_up(button);
                events_->trigger<MouseUpEvent>(button);
                break;
            }
            case SDL_EventType::SDL_MOUSEMOTION: {
                const Vec2<i32> pos { e.motion.x, e.motion.y };
                Input::mouse_.move(pos);
                events_->trigger<MouseMoveEvent>(pos);
                break;
            }
        }
    }
}

void Game::run() {
    init();

    while (true) {
        process_os_messages();
        if (!is_running_) {
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
    }

    shutdown();
}

bool Game::on_script_loaded(ScriptLoadedEvent& e) {
    luabridge::getGlobalNamespace(e.script->lua_state())
        .beginClass<Game>("Env")
            .addFunction("configure", &Game::configure_from_lua)
        .endClass();

    luabridge::setGlobal(e.script->lua_state(), this, "env");
    return false;
}

void Game::configure_from_lua(luabridge::LuaRef cfg) {
    Config cfg_prev = config_;

    constexpr const auto report = [](std::string_view reason) -> void {
        Log::error("Error in root config: {}", reason);
    };

    if (!cfg.isTable()) {
        report("Expected payload to be a table");
        return;
    }

    // TODO: abstract runtime type checking and default value handling
    if (cfg["scaling"].isNumber()) {
        config_.scaling = cfg["scaling"].cast<float_t>();
    } else {
        report("Expected gfx:scaling to be a number");
    }
    if (cfg["font_texture_path"].isString()) {
        config_.font_texture_path = cfg["font_texture_path"].cast<std::string>();
    } else {
        report("Expected gfx:font_texture_path to be a string");
    }

    const auto& glyph_size { cfg["glyph_size"] };
    if (glyph_size.isTable()) {
        if (glyph_size["width"].isNumber()) {
            config_.glyph_size.x = glyph_size["width"];
        } else {
            report("Expected gfx:glyph_size:width to be a number");
        }
        if (glyph_size["height"].isNumber()) {
            config_.glyph_size.y = glyph_size["height"];
        } else {
            report("Expected gfx:glyph_size:height to be a number");
        }
    } else {
        report("Expected gfx:glyph_size to be a table");
    }
    events_->trigger<ConfigUpdatedEvent>(cfg_prev, config_);
}
