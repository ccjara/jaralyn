#ifndef JARALYN_GAME_HXX
#define JARALYN_GAME_HXX

#include "config/config.hxx"
#include "component/skills.hxx"
#include "component/render.hxx"
#include "catalog/catalog.hxx"
#include "ai/ai_walk.hxx"
#include "scripts/api/catalog_api.hxx"
#include "scripts/api/scene_api.hxx"
#include "scripts/api/ui/ui_api.hxx"
#include "scripts/api/log_api.hxx"
#include "xray/log_xray.hxx"
#include "xray/scene_xray.hxx"
#include "xray/script_xray.hxx"
#include "xray/ui_xray.hxx"
#include "input/input.hxx"
#include "ui/ui.hxx"
#include "scripts/script.hxx"
#include "scripts/script_event.hxx"
#include "platform/platform.hxx"
#include "platform/platform_api.hxx"
#include "platform/platform_event.hxx"
#include "gfx/renderer.hxx"
#include "scripts/scripting.hxx"
#include "xray/xray.hxx"
#include "action/action_queue.hxx"
#include "entity/entity.hxx"
#include "entity/entity_manager.hxx"
#include "entity/archetype.hxx"
#include "tile/tile_manager.hxx"
#include "world/world.hxx"

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

    std::unique_ptr<EventManager> events_;
    std::unique_ptr<ActionQueue> action_queue_;
    std::unique_ptr<ServiceLocator> services_ = nullptr;
    std::unique_ptr<World> world_ = nullptr;
    std::unique_ptr<EntityManager> entity_manager_ = nullptr;
    std::unique_ptr<TileManager> tile_manager_ = nullptr;
    std::unique_ptr<Catalog> catalog_ = nullptr;
    std::unique_ptr<Platform> platform_ = nullptr;
};

#endif
