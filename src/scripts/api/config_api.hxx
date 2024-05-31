#ifndef JARALYN_CONFIG_API_HXX
#define JARALYN_CONFIG_API_HXX

#include "scripts/api/lua_api.hxx"

class ConfigManager;

class ConfigApi final : public LuaApi {
public:
    explicit ConfigApi(ConfigManager* config_manager, EventManager* events);

    void on_register(Script* script) final override;
private:
    /**
     * @brief Updates the config from inside a lua script
     */
    void on_configure(luabridge::LuaRef cfg);

    ConfigManager* config_manager_ = nullptr;
    EventManager* events_ = nullptr;
};

#endif
