#ifndef JARALYN_GAME_HXX
#define JARALYN_GAME_HXX

#include "config/config_manager.hxx"
#include "component/skills.hxx"
#include "component/render.hxx"
#include "catalog/catalog.hxx"
#include "ai/ai_walk.hxx"
#include "scripts/api/config_api.hxx"
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
     * @brief Initializes the game and its subsystems
     */
    void init();

    /**
     * @brief Shuts down the game and its subsystems
     */
    void shutdown();

    std::unique_ptr<ConfigManager> config_manager_ = nullptr;
    std::unique_ptr<EventManager> events_ = nullptr;
    std::unique_ptr<ActionQueue> action_queue_ = nullptr;
    std::unique_ptr<ServiceLocator> services_ = nullptr;
    std::unique_ptr<World> world_ = nullptr;
    std::unique_ptr<EntityManager> entity_manager_ = nullptr;
    std::unique_ptr<TileManager> tile_manager_ = nullptr;
    std::unique_ptr<Catalog> catalog_ = nullptr;
    std::unique_ptr<Platform> platform_ = nullptr;
};

#endif
