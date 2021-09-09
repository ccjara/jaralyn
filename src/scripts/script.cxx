#include "script.hxx"

u64 Script::next_id_ { 0 };

Script::Script(const std::string& name) : id { next_id_++ }, name_(name) {
}

Script::~Script() {
    unload();
}

Script::operator lua_State* () const {
    return state_;
}

bool Script::run() {
    if (status_ == ScriptStatus::unloaded) {
        LOG(ERROR) << "Cannot run script " << name_ << ": script must be loaded";
        return false;
    }
    if (lua_pcall(state_, 0, 0, 0) != LUA_OK) {
        fail(ScriptError::runtime_error);
        return false;
    }
    status_ = ScriptStatus::executed;
    return true;
}

void Script::load() {
    if (state_) {
        unload();
    }
    state_ = luaL_newstate();
    if (!state_) {
        return fail(ScriptError::state_alloc_failed);
    }
    if (luaL_loadstring(state_, source_.c_str()) != LUA_OK) {
        unload();
        return fail(ScriptError::script_corrupted);
    }
    luaL_openlibs(state_);
    status_ = ScriptStatus::loaded;
}

void Script::unload() {
    if (state_) {
        lua_close(state_);
        state_ = nullptr;
        status_ = ScriptStatus::unloaded;
    }
    globals_.clear();
    callbacks_.clear();
}

ScriptStatus Script::status() const {
    return status_;
}

const std::string& Script::name() const {
    return name_;
}

lua_State* Script::lua_state() const {
    return state_;
}

const std::vector<std::string>& Script::globals() const {
    return globals_;
}

const std::string& Script::source() const {
    return source_;
}

void Script::set_source(std::string&& source) {
    source_ = std::move(source);
}

void Script::fail(ScriptError err) {
    switch (err) {
    case ScriptError::state_alloc_failed:
        LOG(ERROR) << "Could not allocate new lua state";
        break;
    case ScriptError::runtime_error:
        assert(lua_gettop(state_) == -1);
        LOG(ERROR) << "Runtime error in script " << name_ << ": " << lua_tostring(state_, -1);
        break;
    case ScriptError::script_path_not_found:
        LOG(ERROR) << "Cannot load script " << name_ << ": path " << path_ << " not found";
        break;
    case ScriptError::bad_script_input:
        LOG(ERROR) << "Cannot load script " << name_ << ": bad input";
        break;
    case ScriptError::script_corrupted:
        LOG(ERROR) << "Cannot load script " << name_ << ": " << lua_tostring(state_, -1);
    case ScriptError::none:
        return;
    default:
        LOG(ERROR) << "Unknown error in script " << name_;
    }
    error_ = err;
}
