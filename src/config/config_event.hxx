#ifndef JARALYN_CONFIG_EVENT_HXX
#define JARALYN_CONFIG_EVENT_HXX

#include "config/config.hxx"

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
