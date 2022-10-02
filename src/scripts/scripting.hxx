#ifndef JARALYN_SCRIPT_SYSTEM_HXX
#define JARALYN_SCRIPT_SYSTEM_HXX

#include "../game/platform_event.hxx"
#include "../events/game_event.hxx"
#include "../input/input_event.hxx"
#include "lua_event.hxx"
#include "api/lua_api.hxx"
#include "script_event.hxx"
#include "script_util.hxx"
#include "script.hxx"

class Scripting {
public:
    Scripting() = delete;

    static const inline fs::path default_script_path = fs::absolute(
#ifdef NDEBUG
        "scripts/"
#else
        "../src/scripts/lua/"
#endif
    );

    /**
     * @brief Recursively loads scripts from the given directory path.
     *
     * Script files must have a lowercased `.lua` extension. Scripts inside
     * a `lib` folder will not be loaded. These are intended to be used
     * as modules loadable using the lua `require` command.
     *
     * Script names will be constructed from the file path stem (filename
     * without extension) and its directory level.
     * For each directory level visited, the stem will be prefixed with
     * the visited directory name.
     *
     * Examples (assuming `scripts` as the `base_path`):
     *   - `scripts/extra/optional.lua` -> `extra/optional`
     *   - `scripts/very/deep/file.lua` -> `very/deep/file`
     *   - `scripts/system.lua` -> `system`
     */
    static void load_from_path(const fs::path& base_path);

    static [[nodiscard]] const std::unordered_map<u64, std::unique_ptr<Script>>& scripts();

    static [[nodiscard]] Script* get_by_id(u64 id);

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

    /**
     * @brief Invokes a callback having the given id
     */
    static void invoke_callback(Id callback_id);

    /**
     * @brief Accepts a callable lua to track a lua callback
     *
     * Returns an invokable LuaCallback
     */
    static Id register_lua_callback(luabridge::LuaRef& ref);

    static void register_event_callback(LuaEvent event_type, luabridge::LuaRef ref);

    static void init();
    static void shutdown();
private:
    struct LuaCallback {
        /**
         * @brief Id of the callback, unique across all scripts
         */
        Id id;

        /**
         * @brief Id of the script containing the callback
         */
        Id script_id = null_id;

        /**
         * @brief Lua ref holding the callable
         */
        luabridge::LuaRef ref;
    };

    static bool on_inventory_event(const InventoryEvent& e);

    static inline Id next_callback_id_ = 1;

    static inline std::vector<std::unique_ptr<LuaApi>> apis_;

    static inline std::unordered_map<Id, std::unique_ptr<Script>> scripts_;

    static inline std::unordered_map<Id, LuaCallback> callbacks_;

    static inline std::unordered_map<LuaEvent, std::vector<Id>> event_callbacks_;

    static bool on_key_down(KeyDownEvent& e);

    /**
     * @brief Calls all registered lua callbacks for a specific event type.
     *
     * The arguments are forwarded to each callback.
     */
    template<typename... Args>
    static void invoke_event_callbacks(LuaEvent event_type, Args&&... args) {
        const auto event_callback_iter = event_callbacks_.find(event_type);
        if (event_callback_iter == event_callbacks_.cend()) {
            return;
        }
        for (Id callback_id : event_callback_iter->second) {
            const auto callback_iter = callbacks_.find(callback_id);
            if (callback_iter == callbacks_.cend()) {
                continue;
            }
            pcall_into(callback_iter->second.ref, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Sets the package.path of the given script to the `default_script_path`
     */
    static void inject_package_path(Script& script);

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

template<typename Api, typename... ApiArgs>
void Scripting::add_api(ApiArgs&&... api_args) {
    apis_.emplace_back(new Api(std::forward<ApiArgs>(api_args)...));
}

#endif
