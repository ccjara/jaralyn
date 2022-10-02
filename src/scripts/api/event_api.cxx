#include "event_api.hxx"

void EventApi::on_register(Script* script) {
    luabridge::getGlobalNamespace(*script)
        .beginClass<EventApi>("EventApi")
            .addFunction("on", &EventApi::on)
        .endClass();

    expose(script, this, "events");
}

void EventApi::on(i32 event_type, luabridge::LuaRef ref, lua_State* state) const {
    if (!ref.isFunction()) {
        Log::error("events:on - second parameter must be a function");
        return;
    }
    const auto raw_event_type = static_cast<LuaEvent>(event_type);

    switch (raw_event_type) {
        case LuaEvent::Inventory:
            return Scripting::register_event_callback(raw_event_type, ref);
        default:
            Log::error("Unexpected event type {}", event_type);
            return;
    }
}
