#ifndef JARALYN_SCRIPT_SYSTEM_HXX
#define JARALYN_SCRIPT_SYSTEM_HXX

#include "platform/platform_event.hxx"
#include "input/input_event.hxx"
#include "scripts/api/lua_api.hxx"
#include "scripts/callback_store.hxx"
#include "scripts/script_event.hxx"
#include "scripts/script_util.hxx"
#include "scripts/script.hxx"

using lua_event_type = u32;
namespace lua_event { // FIXME: luabridge does not support enums
    static lua_event_type inventory_view { 1000 };
}

class Scripting {
public:
    Scripting() = delete;

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
    static void load_from_path(path_like base_path);

    [[nodiscard]] static const std::unordered_map<u64, std::unique_ptr<Script>>& scripts();

    [[nodiscard]] static Script* get_by_id(u64 id);

    /**
     * @brief Constructs and registers a lua api fragment in place
     *
     * @see LuaApi
     */
    template<typename Api, typename... ApiArgs>
    static void add_api(ApiArgs&&... api_args);

    /**
     * @brief Setups up globals and namespaces for every script.
     */
    static void setup_script_env(Script& script);

    static void init(EventManager* events);
    static void shutdown();
private:
    static inline std::vector<std::unique_ptr<LuaApi>> apis_;

    static inline std::unordered_map<u64, std::unique_ptr<Script>> scripts_;

    static inline EventManager* events_ = nullptr;

    struct ScriptRef {
        u64 script_id;
        luabridge::LuaRef ref;
    };
    static inline std::unordered_map<lua_event_type, std::vector<ScriptRef>> listeners_;

    static inline CallbackStore callback_store_;

    /**
     * @brief Tracks a lua function that should be invoked on a specific event.
     *
     * The stored refs must be removed before the scripts are unloaded
     */
    static bool register_lua_callback(lua_event_type event_type, luabridge::LuaRef ref);

    static bool on_key_down(KeyDownEvent& e);

    /**
     * @brief Calls all registered lua callbacks for a specific event type.
     *
     * The arguments are forwarded to each callback.
     */
    template<typename... args>
    static void invoke_lua_callbacks(lua_event_type event_type, args&&... arguments) {
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
    static void load(Script& script);

    /**
     * @brief Resets the script system to its initial state
     *
     * In this state, no script or any reference thereof is held
     */
    static void reset();
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
                script->fail(ScriptError::BadScriptInput);
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

template<typename Api, typename... ApiArgs>
void Scripting::add_api(ApiArgs&&... api_args) {
    apis_.emplace_back(new Api(std::forward<ApiArgs>(api_args)...));
}

#endif
