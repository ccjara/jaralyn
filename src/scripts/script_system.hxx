#ifndef JARALYN_SCRIPT_SYSTEM_HXX
#define JARALYN_SCRIPT_SYSTEM_HXX

#include "../system.hxx"
#include "../event/script_event.hxx"
#include "../event/inventory_event.hxx"
#include "../event/platform_event.hxx"
#include "script.hxx"

namespace script_ids {
    constexpr const char* system { "system" };
}

/**
 * @see event_type_by_string
 */
enum class j_public_event_type {
    inventory_item_added,
};

/**
 * @brief Maps strings to j_public_event_type
 *
 * Used when interfacing lua and C++. The string representation should
 * only be used in lua and the enum should be used in C++ at all times.
 *
 * Not every event is necessarily exposed to lua.
 */
static const std::unordered_map<std::string_view, j_public_event_type> event_type_by_string {
    { "INVENTORY_ITEM_ADDED", j_public_event_type::inventory_item_added },
};

class j_script_system : public j_system<j_script_system> {
private:
    struct j_bound_ref {
        std::string script_id;
        luabridge::LuaRef ref;

        j_bound_ref(const char* script_id, luabridge::LuaRef&& ref) :
            script_id { script_id }, ref { std::move(ref) } {
            assert(script_id);
        }
    };

    std::unordered_map<j_public_event_type, std::vector<j_bound_ref>> listeners_;
    std::unordered_map<std::string, j_script> scripts_;

    // item events
    void on_item_stored(const j_item_stored_event& e);

    // internal testing
    void on_key_down(const j_key_down_event& e);

    /**
     * @brief Gets called when any lua function subscribes to any event
     */
    bool on_register_callback(const char* event_type, luabridge::LuaRef ref);

    /**
     * @brief Loads and runs the script
     *
     * TODO: Make scripts loadable on demand
     * TODO: handle reloaded internally by checking if a script was loaded already
     */
    void setup(j_script& script, bool reloaded);

    /**
     * @brief Resets the script system to its initial state
     *
     * In this state, no script or any reference thereof is held
     */
    void reset();

    /**
     * @brief Does a protected call into the given lua ref
     *
     * Checks whether the call was successful. In case of an error, the error
     * is logged, *no* exception is thrown.
     */
    template<typename... varg_t>
    inline void pcall_into(luabridge::LuaRef& ref, varg_t&&... args) const;
public:
    constexpr static const char* default_script_path {
#ifdef NDEBUG
        "scripts"
#else
        "../src/scripts/lua"
#endif
    };

    /**
     * @brief Unloads all scripts before freeing resources
     */
    ~j_script_system();

    /**
     * @brief Recursively preloads all scripts from the given directory path
     *
     * Script files must have a lowercased `.lua` extension.
     *
     * The scripts will be stored as `j_script` instances inside `cache_` and
     * may be retrieved using a script id by calling the `require` method.
     * The id will be constructed from the file path stem (filename without
     * extension) and its directory level.
     * For each directory level visited, the stem will be prefixed with
     * the visited directory name.
     *
     * Examples (assuming `scripts` as the `base_path`):
     *   - `scripts/extra/optional.lua` -> `extra/optional`
     *   - `scripts/very/deep/file.lua` -> `very/deep/file`
     *   - `scripts/system.lua` -> `system`
     */
    template<typename path_like>
    void preload(path_like base_path);

    /**
     * @brief Unloads a script with the given script_id
     *
     * An unloaded script can later be reloaded.
     *
     * @see reload
     */
    template<typename string_like>
    void unload(string_like script_id);

    /**
     * @brief Reloads a script with the given script_id
     *
     * The script must be known to the script engine.
     * Scripts which have been added after {@link preload} was called are not
     * known to the script system (TODO).
     *
     * If the script was loaded from a file, the source will also be reloaded
     * from the file system, making changes at runtime possible.
     */
    template<typename string_like>
    void reload(string_like script_id);

    /**
     * @brief Attempts to load the script with the given script id
     *
     * Panics if the file does not exist
     */
    template<typename string_like>
    j_script& require(string_like script_id);

    void on_load() override;
    void update(uint32_t delta_time) override;
};

template<typename path_like>
void j_script_system::preload(path_like base_path) {
    reset();

    const auto abs_path { fs::absolute(base_path) };

    if (!fs::is_directory(abs_path)) {
        LOG(ERROR) << "Script path " << abs_path << " must be a readable directory";
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
            const std::string& script_id = prefix + path.stem().string();

            LOG(INFO) << "Found script " << script_id;

            // TODO: gracefully handle case insensitive file systems (script ids must be unique)
            // TODO: locale-lowercase script_id for convenience and consistency?
            // TODO: check against unicode file names
            auto [iter, b] = scripts_.try_emplace(script_id, script_id, path);

            setup(iter->second, false);
        }
    }
}

template<typename string_like>
j_script& j_script_system::require(string_like script_id) {
    auto entry { scripts_.find(script_id) };

    if (entry == scripts_.end()) {
        LOG(ERROR) << "Could not find script " << script_id;
        std::abort();
    }

    return entry->second;
}

template<typename... varg_t>
inline void j_script_system::pcall_into(luabridge::LuaRef& ref, varg_t&&... args) const {
    const auto result { ref(std::forward<varg_t>(args)...) };
    if (result == std::nullopt) {
        const auto state { ref.state() };

        std::string err { "Caught error during script execution" };

        if (lua_gettop(state) > 0) {
            const char* e = lua_tostring(state, -1);
            if (e) {
                err.append(": ");
                err.append(e);
            }
        }
        LOG(ERROR) << err;
    }
}

template<typename string_like>
void j_script_system::reload(string_like script_id) {
    auto it = scripts_.find(script_id);
    if (it == scripts_.end()) {
        LOG(ERROR) << "Can not reload unknown script " << script_id;
    }
    unload(script_id);
    setup(it->second, true);
}

template<typename string_like>
void j_script_system::unload(string_like script_id) {
    auto it = scripts_.find(script_id);
    if (it == scripts_.end()) {
        return;
    }
    // OPTIMIZE: this is an extremely slow operation.
    //           improve script_id <-> handle tracking.
    for (auto& [_, refs] : listeners_) {
        refs.erase( // C++20 erase_if
            std::remove_if(refs.begin(), refs.end(), [&script_id](const j_bound_ref& ref) {
                return ref.script_id == script_id;
            }),
            refs.end()
        );
    }
    LOG(INFO) << "Script " << script_id << " has been unloaded";
}

#endif
