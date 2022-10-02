#ifndef JARALYN_EVENT_API_HXX
#define JARALYN_EVENT_API_HXX

#include "../scripting.hxx"

class EventApi final : public LuaApi {
public:
    void on_register(Script* script) final override;

    /**
     * @brief Called from lua to register a callback for a LuaEvent
     */
    void on(i32 event, luabridge::LuaRef ref, lua_State* state) const;
};

#endif
