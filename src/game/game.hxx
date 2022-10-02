#ifndef JARALYN_GAME_HXX
#define JARALYN_GAME_HXX

#include "config.hxx"
#include "window.hxx"
#include "../entity/components/skills.hxx"
#include "../entity/catalog.hxx"
#include "../ai/ai_walk.hxx"
#include "../scripts/api/catalog_api.hxx"
#include "../scripts/api/event_api.hxx"
#include "../scripts/api/scene_api.hxx"
#include "../scripts/api/ui/ui_api.hxx"
#include "../scripts/api/log_api.hxx"
#include "../xray/log_xray.hxx"
#include "../xray/scene_xray.hxx"
#include "../xray/script_xray.hxx"
#include "../xray/ui_xray.hxx"
#include "../input/input.hxx"
#include "../ui/ui.hxx"
#include "../scene/scene.hxx"
#include "../scripts/script.hxx"
#include "../scripts/script_event.hxx"
#include "platform_event.hxx"
#include "../gfx/renderer.hxx"
#include "../scripts/scripting.hxx"
#include "../xray/xray.hxx"

class Game {
public:
    /**
     * @brief Runs the game loop until stopped by the player.
     */
    void run();
private:
    /**
     * @brief The source of truth of the system configuration.
     *
     * Some services may claim a copy of this config which gets distributed by
     * the dispatcher.
     *
     * @see ConfigUpdatedEvent
     */
    Config config_;

    /**
     * @brief Terminator for the game loop.
     */
    bool is_running_ { false };

    /**
     * @brief Initializes the game and its subsystems
     */
    void init();

    /**
     * @brief Shuts down the game and its subsystems
     */
    void shutdown();

    /**
     * @brief Polls for pending OS messages and dispatches env events.
     *
     * @see env_event_dispatcher
     */
    void process_os_messages();

    /**
     * @brief Updates the config from inside a lua script.
     */
    void configure_from_lua(luabridge::LuaRef cfg);

    bool on_script_loaded(ScriptLoadedEvent&);
};

#endif
