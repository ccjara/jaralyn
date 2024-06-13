#ifndef NOEDEN_WORLD_HXX
#define NOEDEN_WORLD_HXX

#include "action/action_event.hxx"
#include "world/camera_controller.hxx"

class IEntityReader;
class IPlayerController;
class IActionProcessor;
class Entity;
struct Camera;

class World {
public:
    explicit World(IEntityReader* entity_reader, IActionProcessor* action_processor, Events* events);

    /**
     * @brief Sets the player controller for this world
     * 
     * @param controller Player controller to bind
     */
    void bind_player_controller(IPlayerController* controller);

    const Camera& get_camera() const;

    CameraController& get_camera_controller();
private:
    EventResult on_player_action_committed(const PlayerActionCommitted& e);
    Subscription<PlayerActionCommitted> player_action_committed_sub_;

    /**
     * @brief Player controller bound to this world
     */
    IPlayerController* player_controller = nullptr;

    Events* events_ = nullptr;
    IEntityReader* entity_reader_ = nullptr;
    IActionProcessor* action_processor_ = nullptr;

    std::unique_ptr<Camera> camera_ = nullptr;
    std::unique_ptr<CameraController> camera_controller_ = nullptr;
};

#endif
