#include "env_manager.hxx"

j_env_manager::j_env_manager(entt::dispatcher* const dispatcher) : dispatcher_(dispatcher) {
    assert(dispatcher_);

    dispatcher_->sink<j_quit_event>().connect<&j_env_manager::on_quit>(this);
    dispatcher_->sink<j_resize_event>().connect<&j_env_manager::on_resize>(this);
    dispatcher_->sink<j_script_loaded_event>().connect<&j_env_manager::on_script_loaded>(this);
}

j_env_manager::~j_env_manager() {
    if (is_running_) {
        stop();
    }
}

void j_env_manager::start() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(ERROR) << "SDL could not initialize! (" << SDL_GetError() << ")";
        std::abort();
    }

    SDL_Rect display_bounds;
    SDL_GetDisplayBounds(0, &display_bounds);

    window_ = std::make_unique<j_window>(j_vec2<uint32_t>{
        static_cast<uint32_t>(display_bounds.w) / 2,
        static_cast<uint32_t>(display_bounds.h) / 2
    }, "Jaralyn");

    is_running_ = true;
}

void j_env_manager::on_quit(const j_quit_event&) {
    stop();
}

void j_env_manager::on_resize(const j_resize_event& e) {
    window_->resize(e.size);
}

void j_env_manager::on_script_loaded(const j_script_loaded_event& e) {
    luabridge::getGlobalNamespace(e.script->lua_state())
        .beginClass<j_env_manager>("Env")
            .addFunction("configure", &j_env_manager::configure_from_lua)
        .endClass();

    luabridge::setGlobal(e.script->lua_state(), this, "env");
}

void j_env_manager::configure_from_lua(luabridge::LuaRef cfg) {
    j_root_config cfg_prev = root_config_;

    constexpr const auto report = [](std::string_view reason) -> void {
        LOG(ERROR) << "Error in root config: " << reason << ".";
    };

    if (!cfg.isTable()) {
        report("Expected payload to be a table");
        return;
    }

    // TODO: abstract runtime type checking and default value handling
    if (cfg["scaling"].isNumber()) {
        root_config_.scaling = cfg["scaling"].cast<float_t>();
    } else {
        report("Expected gfx:scaling to be a number");
    }
    if (cfg["font_texture_path"].isString()) {
        root_config_.font_texture_path = cfg["font_texture_path"].cast<std::string>();
    } else {
        report("Expected gfx:font_texture_path to be a string");
    }

    const auto& glyph_size { cfg["glyph_size"] };
    if (glyph_size.isTable()) {
        if (glyph_size["width"].isNumber()) {
            root_config_.glyph_size.x = glyph_size["width"];
        } else {
            report("Expected gfx:glyph_size:width to be a number");
        }
        if (glyph_size["height"].isNumber()) {
            root_config_.glyph_size.y = glyph_size["height"];
        } else {
            report("Expected gfx:glyph_size:height to be a number");
        }
    } else {
        report("Expected gfx:glyph_size to be a table");
    }
    dispatcher_->trigger(j_root_config_updated_event(std::move(cfg_prev), root_config_));
}

const j_root_config& j_env_manager::config() const {
    return root_config_;
}

bool j_env_manager::running() const {
    return is_running_;
}

void j_env_manager::stop() {
    window_->close();
    SDL_Quit();
    is_running_ = false;
}

j_clock& j_env_manager::clock() {
    return clock_;
}

j_window& j_env_manager::window() {
    return *window_;
}

void j_env_manager::process_os_messages() const {
    SDL_PumpEvents();

    SDL_Event e;
    while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_EventType::SDL_FIRSTEVENT, SDL_EventType::SDL_WINDOWEVENT) != 0) {
        switch (e.type) {
        case SDL_EventType::SDL_QUIT:
            dispatcher_->trigger<j_quit_event>();
            return;
        case SDL_EventType::SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                dispatcher_->trigger<j_resize_event>(j_vec2<uint32_t> {
                    static_cast<uint32_t> (e.window.data1),
                    static_cast<uint32_t> (e.window.data2)
                });
            }
            break;
        }
    }
}
