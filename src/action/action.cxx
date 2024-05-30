#include "action/action.hxx"

Entity *Action::entity() const {
    return entity_;
}

u32 Action::cost() const {
    return cost_;
}

u32 Action::speed() const {
    return speed_;
}

ActionType Action::type() const {
    return type_;
}
