#ifndef NOEDEN_CAMERA_CONTROLLER_HXX
#define NOEDEN_CAMERA_CONTROLLER_HXX

#include "framework/engine_event_type.hxx"
#include "world/world_event.hxx"
#include "input/input_event.hxx"
#include "gfx/gfx_event.hxx"

struct Camera;
class Events;
class Entity;
class IEntityReader;

class CameraController {
public:
    explicit CameraController(IEntityReader* entity_reader, Events* events);

    /**
     * @brief Assigns camera to be controlled
     */
    void control(Camera* camera);

    /**
     * @brief Assigns entity to follow
     */
    void set_target(Entity* entity);

    Entity* target() const;
private:
    void adjust(const WorldPos& position);

    EventResult on_key_down(const KeyDownEvent& e);
    Subscription<KeyDownEvent> key_down_sub_;
    EventResult on_world_updated_post(const WorldUpdatedPostEvent& e);
    Subscription<WorldUpdatedPostEvent> world_updated_post_sub_;
    EventResult on_world_ready(const WorldReadyEvent& e);
    Subscription<WorldReadyEvent> world_ready_sub_;
    EventResult on_display_resized(const DisplayResizedEvent& e);
    Subscription<DisplayResizedEvent> display_resized_sub_;

    /**
     * @brief Entity that the camera is following
     */
    Entity* target_ = nullptr;

    IEntityReader* entity_reader_ = nullptr;
    Camera* camera_ = nullptr;
    Events* events_ = nullptr;

    i32 world_width_ = 0;
    i32 world_height_ = 0;

    Vec2<i32> dispay_size_;
};

#endif