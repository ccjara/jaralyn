#include "scripting.hxx"

void Scripting::init() {
    Events::on<KeyDownEvent>(&Scripting::on_key_down);
    Events::on<InventoryEvent>(&Scripting::on_inventory_event);
}

void Scripting::shutdown() {
    callbacks_.clear();
    event_callbacks_.clear();
    scripts_.clear();
}

void Scripting::reset() {
    Events::trigger<ScriptResetEvent>();
    callbacks_.clear();
    event_callbacks_.clear();
    scripts_.clear();
}

bool Scripting::on_inventory_event(const InventoryEvent& e) {
    invoke_event_callbacks(LuaEvent::Inventory, e.entity_id);
    return false;
}

bool Scripting::on_key_down(KeyDownEvent& e) {
    if (e.key == Key::F5) {
        load_from_path(default_script_path);
        return true;
    }
    return false;
}

void Scripting::load_from_path(const fs::path& base_path) {
    if (!scripts_.empty()) {
        reset();
    }
    if (!fs::is_directory(base_path)) {
        Log::error("Script path {} must be a readable directory", base_path.string());
        std::abort();
    }
    // generic path string up to (but not including the file name)
    std::string prefix;
    // default options: does not follow symlinks, skips on denied permission
    fs::recursive_directory_iterator dir_iter(base_path);

    for (decltype(dir_iter) end; dir_iter != end; ++dir_iter) {
        if (dir_iter.depth() == 0) {
            prefix = "";
        }
        const auto& entry = *dir_iter;
        const auto& path = entry.path();
        const auto filename = path.filename().string();
        if (entry.is_directory()) {
            if (filename == "lib") { // skip lib folders, these are used for lua packaging
                dir_iter.disable_recursion_pending();
            } else {
                prefix += filename + "/";
            }
            continue;
        }
        if (path.extension().string() == ".lua") {
            std::string name = prefix + path.stem().string();
            Log::info("Found script file `{}`", name);

            auto script = std::make_unique<Script>(std::move(name));
            script->path_ = std::move(path.string());
            std::ifstream input(path, std::ios::ate);
            if (input.bad()) {
                script->fail(ScriptError::BadScriptInput);
            }
            const auto size = input.tellg();
            script->source_.resize(size);
            input.seekg(0);
            input.read(script->source_.data(), size);
            auto [iter, b] = scripts_.try_emplace(script->id, std::move(script));
            load(*iter->second);
        }
    }
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
    Events::trigger<ScriptLoadedEvent>(&script);
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

void Scripting::invoke_callback(Id callback_id) {
    auto it = callbacks_.find(callback_id);
    if (it == callbacks_.end()) {
        Log::warn("Requested callback {} does not exist for invocation.", callback_id);
        return;
    }
    it->second.ref();
}

void Scripting::register_event_callback(LuaEvent type, luabridge::LuaRef ref) {
    Id callback_id = register_lua_callback(ref);
    if (callback_id != null_id) {
        event_callbacks_[type].push_back(callback_id);
    }
}

Id Scripting::register_lua_callback(luabridge::LuaRef& ref) {
    const auto script_id_ref = luabridge::getGlobal(ref.state(), "script_id");
    if (!script_id_ref.isNumber()) {
        return null_id;
    }
    const auto script_id = script_id_ref.cast<u64>();
    if (!ref.isFunction()) {
        Log::error("Could not register lua callback from script {}: Lua callback must be a function", script_id);
        return null_id;
    }
    Script* script = get_by_id(script_id);
    if (!script) {
        Log::error("Could not find script {}", script_id);
        return null_id;
    }

    const Id callback_id = next_callback_id_++;
    callbacks_.try_emplace(callback_id, LuaCallback{callback_id, script_id, ref});
    Log::info("Lua callback {} registered in script {}", callback_id, script_id);
    return callback_id;
}

void Scripting::setup_script_env(Script& script) {
    // when calling back from lua to engine code it is often useful to identify
    // the calling script. not sure how else to do it.
    luabridge::setGlobal(script, script.id, "script_id");               // TODO: ensure immutability
    luabridge::setGlobal(script, script.name().c_str(), "script_name"); // TODO: ensure immutability

    inject_package_path(script);

    luabridge::getGlobalNamespace(script)
        .beginClass<Scripting>("Script")
            .addStaticFunction("on", &Scripting::register_event_callback)
        .endClass();

    // expose all registered apis
    for (auto& api : apis_) {
        api->on_register(&script);
    }
}

void Scripting::inject_package_path(Script& script) {
    auto package_ref = luabridge::getGlobal(script, "package");

    if (!package_ref.isTable()) {
        Log::warn("`package` global does not exist in script {}", script.name());
        return;
    }
    auto package_path_ref = package_ref["path"];
    if (!package_path_ref.isString()) {
        Log::warn("`package.path` corrupted in script {}", script.name());
        return;
    }
    std::string current_path = package_path_ref.cast<std::string>();
    current_path.append(";");
    current_path.append(default_script_path.generic_string());
    current_path.append("?.lua");
    package_path_ref = current_path;
}