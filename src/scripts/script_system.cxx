#include "script_system.hxx"

j_script_system::~j_script_system() {
    reset();
}

void j_script_system::on_load() {
    dispatcher_->sink<j_key_down_event>().connect<&j_script_system::on_key_down>(this);
}

void j_script_system::reset() {
    for (auto& [id, script] : scripts_) {
        unload(*script.get());
    }
    scripts_.clear();
}

void j_script_system::update(uint32_t delta_time) {
}

void j_script_system::on_key_down(const j_key_down_event& e) {
    if (e.key == SDLK_F5) {
        reset();
        preload(default_script_path);
    }
}

void j_script_system::load(j_script& script) {
    if (script.status() != j_script_status::unloaded) {
        LOG(ERROR) << "Cannot load script "
                   << script.name() << ": script is already loaded. "
                   << "Call reload instead.";
        return;
    }
    script.load();
    if (script.status() != j_script_status::loaded) {
        return;
    }
    LOG(INFO) << "Script " << script.name() << " (" << script.id() << ") has been loaded";
    // allow other parts of the system to contribute to the scripting env
    dispatcher_->trigger<j_script_loaded_event>(&script);
    script.run();
    // execute the on_load function, passing the script env proxy
    auto on_load { luabridge::getGlobal(script, "on_load") };
    if (on_load.isFunction()) {
        pcall_into(on_load);
    }
}

void j_script_system::load(j_id_t id) {
    auto it = scripts_.find(id);
    if (it == scripts_.end()) {
        LOG(ERROR) << "Can not load script " << id << ": script does not exist";
        return;
    }
    return load(*it->second.get());
}

void j_script_system::unload(j_id_t id) {
    auto it = scripts_.find(id);
    if (it == scripts_.end()) {
        LOG(ERROR) << "Can not unload script " << id << ": script does not exist";
        return;
    }
    return unload(*it->second.get());
}

void j_script_system::unload(j_script& script) {
    if (script.status() == j_script_status::unloaded) {
        return;
    }
    dispatcher_->trigger<j_script_before_unload_event>(&script);
    script.unload();
    LOG(INFO) << "Script " << script.name() << " (" << script.id() << ") has been unloaded";
}

void j_script_system::reload(j_id_t id) {
    auto it = scripts_.find(id);
    if (it == scripts_.end()) {
        LOG(ERROR) << "Can not reload script " << id << ": script does not exist";
        return;
    }
    reload(*it->second.get());
}

void j_script_system::reload(j_script& script) {
    unload(script);
    load(script);
}

const std::unordered_map<j_id_t, std::unique_ptr<j_script>>& j_script_system::scripts() const {
    return scripts_;
}
