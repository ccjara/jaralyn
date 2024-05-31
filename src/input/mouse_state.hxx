#ifndef JARALYN_MOUSE_STATE_HXX
#define JARALYN_MOUSE_STATE_HXX

#include "input/mouse_button.hxx"

/**
 * @brief Buffers the mouse state
 */
class MouseState {
public:
    MouseState();

    Vec2<i32> position() const;

    i32 x() const;
    i32 y() const;

    /**
     * @brief Sets the mouse position (absolute)
     */
    void move(Vec2<i32> pos);

    /**
     * @brief Updates the state of the given button to `pressed`
     */
    void mouse_down(MouseButton button);

    /**
     * @brief Updates the state of the given button to `released`
     */
    void mouse_up(MouseButton button);

    /**
     * @brief Returns whether the given button is currently pressed
     */
    bool is_pressed(MouseButton button) const;
private:
    Vec2<i32> position_;
    std::array<bool, MOUSE_BUTTON_COUNT> button_state_;

    constexpr u32 button_to_index(MouseButton button) const;
};

#endif
