#ifndef NOEDEN_ENGINE_EVENT_TYPE_HXX
#define NOEDEN_ENGINE_EVENT_TYPE_HXX

enum class EngineEventType {
    None,
    ConfigUpdated,
    MouseMove,
    MouseDown,
    MouseUp,
    KeyDown,
    KeyUp,
    Resize,
    DisplayResized,
    EntityCreated,
    ScriptReset,
    ScriptLoaded,
    ScriptEnvSetup,
    ActionQueueProcessed,
    PlayerActionCommitted,
    PlayerMoved,
    WorldReady,
    WorldUpdatedPre,
    WorldUpdatedPost,
};

#endif
