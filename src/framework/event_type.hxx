#ifndef JARALYN_EVENT_TYPE_HXX
#define JARALYN_EVENT_TYPE_HXX

enum class EventType {
    None,
    ConfigUpdated,
    MouseMove,
    MouseDown,
    MouseUp,
    KeyDown,
    KeyUp,
    Resize,
    ScriptReset,
    ScriptLoaded,
    ActionQueueProcessed,
    DisplayResized,
};

#endif
