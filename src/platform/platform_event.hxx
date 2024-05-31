#ifndef JARALYN_PLATFORM_EVENT_HXX
#define JARALYN_PLATFORM_EVENT_HXX

#include "config/config.hxx"

/**
 * @brief Triggered as the user resizes the window.
 */
struct ResizeEvent {
    Vec2<i32> size;
};

#endif
