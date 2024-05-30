#ifndef JARALYN_PLATFORM_EVENT_HXX
#define JARALYN_PLATFORM_EVENT_HXX

#include "config/config.hxx"

/**
 * @brief Triggered as the user resizes the window.
 */
struct ResizeEvent {
    Vec2<i32> size;
};

/**
 * @brief Triggered every time the configuration updates (usually from within lua)
 */
struct ConfigUpdatedEvent {
    Config prev;
    Config next;

    explicit ConfigUpdatedEvent(Config& cfg_prev, Config& cfg_next) :
        prev(cfg_prev),
        next(cfg_next) {
    }
};

#endif
