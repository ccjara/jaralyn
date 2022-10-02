#ifndef JARALYN_SCRIPT_HXX
#define JARALYN_SCRIPT_HXX

#include "script_util.hxx"

enum class ScriptStatus {
    Unloaded,
    Loaded,
    Executed,
};

enum class ScriptError {
    None,
    RuntimeError,
    StateAllocFailed,
    ScriptPathNotFound,
    BadScriptInput,
    ScriptCorrupted,
};

/**
 * @brief Wrapper which manages a lua state
 */
class Script {
    friend class Scripting;
public:
    u64 id;

    /**
     * @brief Instantiates a script from a source string
     */
    explicit Script(const std::string& name);

    /**
     * @brief Frees the currently managed lua state if allocated
     */
    ~Script();

    /**
     * @brief Implicit conversion from a script instance to a lua state
     *
     * This allows passing a Script to lua C functions.
     */
    operator lua_State* () const;

    /**
     * @brief Runs the script by doing a lua pcall
     *
     * Returns false if any error occurred during execution
     */
    bool run();

    /**
     * @brief Loads the script from its configured data source
     *
     * Sets the script status to `loaded` if loaded successfully or
     * `error` on failure. This also sets the `error_` member accordingly.
     */
    void load();

    /**
     * @brief Unloads the script, freeing its resources
     */
    void unload();

    template<typename... types>
    inline void declare() {
        (types::declare(luabridge::getGlobalNamespace(state_)), ...);
    }

    ScriptStatus status() const;
    const std::string& name() const;
    lua_State* lua_state() const;
    const std::string& source() const;
    void set_source(std::string&& source);

    Script(Script&&) = delete;
    Script& operator=(Script&&) = delete;
    Script(const Script&) = delete;
    Script& operator=(const Script&) = delete;
private:
    static inline u64 next_id_ = 1;

    std::string name_;
    lua_State* state_ = nullptr;
    ScriptStatus status_ = ScriptStatus::Unloaded;
    ScriptError error_ = ScriptError::None;

    std::string path_;
    std::string source_;

    void fail(ScriptError err);
};

#endif
