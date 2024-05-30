#include "scripting.hxx"

void Scripting::init(EventManager* events) {
    assert(events);
    events_ = events;
    events_->on<KeyDownEvent>(&Scripting::on_key_down);
}

void Scripting::shutdown() {
    listeners_.clear();
    scripts_.clear();
}

void Scripting::reset() {
    events_->trigger<ScriptResetEvent>();
    listeners_.clear();
    scripts_.clear();
}

bool Scripting::on_key_down(KeyDownEvent& e) {
    if (e.key == Key::F5) {
        load_from_path(default_script_path);
        return true;
    }
    return false;
}

void Scripting::load(Script& script) {
    if (script.status() != ScriptStatus::Unloaded) {
        Log::error("Could not load script {}: script is already loaded.", script.name());
        return;
    }
    script.load();
    if (script.status() != ScriptStatus::Loaded) {
        return;
    }
    setup_script_env(script);

    // allow other parts of the system to contribute to the scripting env
    events_->trigger<ScriptLoadedEvent>(&script);
    Log::info("Script #{}: {} has been loaded", script.id, script.name());
    script.run();
    // execute the on_load function, passing the script env proxy
    auto on_load { luabridge::getGlobal(script, "on_load") };
    if (on_load.isFunction()) {
        pcall_into(on_load);
    }
}

const std::unordered_map<u64, std::unique_ptr<Script>>& Scripting::scripts() {
    return scripts_;
}

Script* Scripting::get_by_id(u64 id) {
    const auto it { scripts_.find(id) };
    return it == scripts_.end() ? nullptr : it->second.get();
}

bool Scripting::register_lua_callback(lua_event_type event_type, luabridge::LuaRef ref) {
    const auto script_id {
        luabridge::getGlobal(ref.state(), "script_id").cast<u64>()
    };
    if (!ref.isFunction()) {
        Log::error("Could not register lua callback for {} on script #{}: Lua callback must be a function", event_type, script_id);
        return false;
    }
    listeners_[event_type].emplace_back(ScriptRef{ script_id, ref });
    Log::info("Lua callback for {} registered in script #{}", event_type, script_id);
    return true;
}

void Scripting::setup_script_env(Script& script) {
    // when calling back from lua to engine code it is often useful to identify
    // the calling script. not sure how else to do it.
    luabridge::setGlobal(script, script.id, "script_id"); // TODO: ensure immutability
    luabridge::setGlobal(script, script.name().c_str(), "script_name"); // TODO: ensure immutability

    luabridge::getGlobalNamespace(script)
        .beginClass<Scripting>("Script")
            .addStaticFunction("on", &Scripting::register_lua_callback)
        .endClass();
    // expose all registered apis
    for (auto& api : apis_) {
        api->on_register(&script);
    }
}
