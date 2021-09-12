#ifndef JARALYN_SCRIPT_SYSTEM_HXX
#define JARALYN_SCRIPT_SYSTEM_HXX

#include "../game/platform_event.hxx"
#include "../input/input_event.hxx"
#include "api/api_registrar.hxx"
#include "script_event.hxx"
#include "script_util.hxx"
#include "script.hxx"

using lua_event_type = u32;
namespace lua_event { // FIXME: luabridge does not support enums
    static lua_event_type inventory_view { 1000 };
}

class Scripting {
public:
    Scripting(entt::dispatcher& dispatcher);

    constexpr static const char* default_script_path {
#ifdef NDEBUG
        "scripts"
#else
        "../src/scripts/lua"
#endif
    };

    /**
     * @brief Recursively loads all scripts from the given directory path.
     *
     * Script files must have a lowercased `.lua` extension.
     *
     * The script name will be constructed from the file path stem (filename
     * without extension) and its directory level.
     * For each directory level visited, the stem will be prefixed with
     * the visited directory name.
     *
     * Examples (assuming `scripts` as the `base_path`):
     *   - `scripts/extra/optional.lua` -> `extra/optional`
     *   - `scripts/very/deep/file.lua` -> `very/deep/file`
     *   - `scripts/system.lua` -> `system`
     */
    template<typename path_like>
    void load_from_path(path_like base_path);

    [[nodiscard]] const std::unordered_map<u64, std::unique_ptr<Script>>& scripts() const;

    Script* get_by_id(u64 id) const;

    /**
     * @brief Setups up globals and namespaces for every script.
     */
    void setup_script_env(Script& script);

    ApiRegistrar& registrar();

    void startup();
    void shutdown();
private:
    entt::dispatcher& dispatcher_;
    ApiRegistrar api_registrar_;

    std::unordered_map<u64, std::unique_ptr<Script>> scripts_;

    struct ScriptRef {
        u64 script_id;
        luabridge::LuaRef ref;
    };
    std::unordered_map<lua_event_type, std::vector<ScriptRef>> listeners_;

    /**
     * @brief Tracks a lua function that should be invoked on a specific event.
     *
     * The stored refs must be removed before the scripts are unloaded
     */
    bool register_lua_callback(lua_event_type event_type, luabridge::LuaRef ref);

    void on_key_down(const KeyDownEvent& e);

    /**
     * @brief Calls all registered lua callbacks for a specific event type.
     *
     * The arguments are forwarded to each callback.
     */
    template<typename... args>
    void invoke_lua_callbacks(lua_event_type event_type, args&&... arguments) {
        const auto it { listeners_.find(event_type) };
        if (it == listeners_.end()) {
            return;
        }
        for (auto& script_ref : it->second) {
            pcall_into(script_ref.ref, std::forward<args>(arguments)...);
        }
    }

    /**
     * @brief Loads a script after collecting all scripts in {@see load_from_path}
     */
    void load(Script& script);

    /**
     * @brief Resets the script system to its initial state
     *
     * In this state, no script or any reference thereof is held
     */
    void reset();
};

template<typename path_like>
void Scripting::load_from_path(path_like base_path) {
    if (!scripts_.empty()) {
        reset();
    }
    const auto abs_path { fs::absolute(base_path) };

    if (!fs::is_directory(abs_path)) {
        Log::error("Script path {} must be a readable directory", abs_path.string());
        std::abort();
    }

    std::string prefix;

    // default options: does not follow symlinks, skips on denied permission
    for (const auto& entry : fs::recursive_directory_iterator(abs_path)) {
        const auto& path { entry.path() };
        const auto filename { path.filename().string() };

        if (entry.is_directory()) {
            prefix += filename + "/";
            continue;
        }
        if (path.extension().string() == ".lua") {
            const auto& name { prefix + path.stem().string() };
            Log::info("Found script file {}", name);

            auto script { std::make_unique<Script>(name) };
            script->path_ = path.string().c_str();
            std::ifstream input { path, std::ios::ate };
            if (input.bad()) {
                script->fail(ScriptError::bad_script_input);
            }
            const auto size { input.tellg() };
            script->source_.resize(size);
            input.seekg(0);
            input.read(script->source_.data(), size);
            // TODO: gracefully handle case insensitive file systems (script names must be unique)
            // TODO: locale-lowercase script id for convenience and consistency?
            // TODO: check against unicode file names
            auto [iter, b] { scripts_.try_emplace(script->id, std::move(script)) };
            load(*iter->second);
        }
    }
}

#endif
