#ifndef JARALYN_PLATFORM_EVENT_HXX
#define JARALYN_PLATFORM_EVENT_HXX

#include "framework/event_type.hxx"
#include "config/config.hxx"

/**
 * @brief Triggered as the user resizes the window.
 */
struct ResizeEvent {
    static constexpr EventType event_type = EventType::Resize;

    Vec2<i32> size;
};

#endif
