#ifndef JARALYN_SCRIPT_EVENT_HXX
#define JARALYN_SCRIPT_EVENT_HXX

#include "framework/event_type.hxx"

class Script;

/**
 * @brief Triggered prior to resetting all scripts
 */
struct ScriptResetEvent {
    static constexpr EventType event_type = EventType::ScriptReset;
};

/**
 * @brief Triggered after a script loaded
 */
struct ScriptLoadedEvent {
    static constexpr EventType event_type = EventType::ScriptLoaded;

    Script* script { nullptr };

    explicit constexpr ScriptLoadedEvent(Script* script) : script { script } {
        assert(this->script);
    }
};

#endif
