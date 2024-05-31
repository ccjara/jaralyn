#include "input/input.hxx"
#include "input/input_event.hxx"

Input::Input(EventManager* events) : events_(events) {
    assert(events_);
}

Vec2<u32> Input::mouse_position() {
    return mouse_.position();
}

u32 Input::mouse_x() const {
    return mouse_.x();
}

u32 Input::mouse_y() const {
    return mouse_.y();
}

bool Input::is_mouse_pressed(MouseButton button) const {
    return mouse_.is_pressed(button);
}

bool Input::is_key_pressed(Key key) const {
    return keyboard_.is_pressed(key);
}

void Input::set_mouse_position(Vec2<i32> pos) {
    mouse_.move(pos);
    events_->trigger<MouseMoveEvent>(pos);
}

void Input::set_mouse_button_pressed(MouseButton button, bool pressed) {
    if (pressed) {
        mouse_.mouse_down(button);
        events_->trigger<MouseDownEvent>(button);
    } else {
        mouse_.mouse_up(button);
        events_->trigger<MouseUpEvent>(button);
    }
}

void Input::set_key_pressed(Key key, bool pressed) {
    if (pressed) {
        keyboard_.key_down(key);
        events_->trigger<KeyDownEvent>(key);
    } else {
        keyboard_.key_up(key);
        events_->trigger<KeyUpEvent>(key);
    }
}
