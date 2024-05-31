#include "input/mouse_state.hxx"

MouseState::MouseState() {
    std::fill(button_state_.begin(), button_state_.end(), false);
}

Vec2<i32> MouseState::position() const {
    return position_;
}

void MouseState::move(Vec2<i32> pos) {
    position_ = pos;
}

void MouseState::mouse_down(MouseButton button) {
    button_state_[button_to_index(button)] = true;
}

void MouseState::mouse_up(MouseButton button) {
    button_state_[button_to_index(button)] = false;
}

bool MouseState::is_pressed(MouseButton button) const {
    return button_state_.at(button_to_index(button));
}

i32 MouseState::x() const {
    return position_.x;
}

i32 MouseState::y() const {
    return position_.y;
}

constexpr u32 MouseState::button_to_index(MouseButton button) const {
    return static_cast<u32>(button) % MOUSE_BUTTON_COUNT;
}