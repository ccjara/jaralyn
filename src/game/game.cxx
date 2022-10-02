#include "game.hxx"

void Game::init() {
    Log::init();
    Events::init();
    Events::on<ScriptLoadedEvent>(this, &Game::on_script_loaded);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Log::error("SDL could not initialize: {}", SDL_GetError());
        std::abort();
    }

    SDL_Rect display_bounds;
    SDL_GetDisplayBounds(0, &display_bounds);

    Window::open(Vec2<u32>{
        static_cast<u32>(static_cast<float>(display_bounds.w) / 1.25f),
        static_cast<u32>(static_cast<float>(display_bounds.h) / 1.25f)
    }, "Jaralyn");


    Renderer::init();
    Ui::init();
    Scene::init();
    Scripting::init();

    Translator::load("en");

    // xray / engine ui
    Xray::init(Renderer::gl_context());
    Xray::add<LogXray>();
    Xray::add<SceneXray>();
    Xray::add<ScriptXray>();
    Xray::add<UiXray>();

    // scripting
    Scripting::add_api<LogApi>();
    Scripting::add_api<EventApi>();
    Scripting::add_api<SceneApi>();
    Scripting::add_api<UiApi>();
    Scripting::add_api<CatalogApi>();

    Scripting::load_from_path(Scripting::default_script_path);

    is_running_ = true;

    // post initialization experimentation
    {
        auto arch_troll = Catalog::archetype("TROLL");
        auto arch_dwarf = Catalog::archetype("DWARF");
        if (arch_troll) {
            auto& troll = Scene::create_entity(*arch_troll);
            troll.name = "Troll";
            troll.position = { 3, 3 };
            //troll.ai.add<AiWalk>(0, &troll);
            // player.add_component<Skills>();
        } else {
            Log::warn("TROLL archetype not yet present");
        }
        if (arch_dwarf) {
            auto& dwarf = Scene::create_entity(*arch_dwarf);
            dwarf.position = { 0, 1 };
            dwarf.name = "Dwarf";
            Scene::set_player(dwarf.id);
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
    Window::close();
    SDL_Quit();
    Events::shutdown();
}

void Game::process_os_messages() {
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
                    Window::resize(Vec2<u32> {
                        static_cast<u32> (e.window.data1),
                        static_cast<u32> (e.window.data2)
                    });
                    Events::trigger<ResizeEvent>(Window::size());
                }
                break;
            }
            case SDL_EventType::SDL_KEYDOWN: {
                const Key key { static_cast<Key>(e.key.keysym.sym) };
                Input::keyboard_.key_down(key);
                Events::trigger<KeyDownEvent>(key);
                break;
            }
            case SDL_EventType::SDL_KEYUP: {
                const auto key { static_cast<Key>(e.key.keysym.sym) };
                Input::keyboard_.key_up(key);
                Events::trigger<KeyUpEvent>(key);
                break;
            }
            case SDL_EventType::SDL_MOUSEBUTTONDOWN: {
                const auto button { static_cast<MouseButton>(e.button.button) };
                Input::mouse_.mouse_down(button);
                Events::trigger<MouseDownEvent>(button);
                break;
            }
            case SDL_EventType::SDL_MOUSEBUTTONUP: {
                const auto button { static_cast<MouseButton>(e.button.button) };
                Input::mouse_.mouse_up(button);
                Events::trigger<MouseUpEvent>(button);
                break;
            }
            case SDL_EventType::SDL_MOUSEMOTION: {
                const Vec2<i32> pos { e.motion.x, e.motion.y };
                Input::mouse_.move(pos);
                Events::trigger<MouseMoveEvent>(pos);
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
        // world clock advances upon player commands
        Scene::perform_actions();
        // update engine submodules
        Ui::update();
        // render
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
        Log::error("Error in root config: {}.", reason);
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
    Events::trigger<ConfigUpdatedEvent>(cfg_prev, config_);
}
