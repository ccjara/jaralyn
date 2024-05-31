#ifndef JARALYN_ACTION_EVENT_HXX
#define JARALYN_ACTION_EVENT_HXX

#include "framework/event_type.hxx"

struct ActionQueueProcessed {
    static constexpr EventType event_type = EventType::ActionQueueProcessed;
};

#endif
