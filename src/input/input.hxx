#ifndef JARALYN_INPUT_HXX
#define JARALYN_INPUT_HXX

#include "input/mouse_state.hxx"
#include "input/keyboard_state.hxx"
#include "input/input_reader.hxx"

/**
 * @brief Input state provder
 */
class Input : public IInputReader {
public:
    explicit Input(EventManager* events);

    /**
     * @brief Returns the current mouse position as a tuple
     */
    Vec2<u32> mouse_position() override;

    /**
     * @brief Returns the current x coordinate of the mouse cursor
     */
    u32 mouse_x() const override;

    /**
     * @brief Returns the current y coordinate of the mouse cursor
     */
    u32 mouse_y() const override;

    /**
     * @brief Returns true if the given mouse button is currently pressed
     */
    bool is_mouse_pressed(MouseButton button) const override;

    /**
     * @brief Returns true if the given keyboard key is currently pressed
     */
    bool is_key_pressed(Key key) const override;

    /**
     * @brief Sets the mouse position (absolute)
     */
    void set_mouse_position(Vec2<i32> pos);

    /**
     * @brief Updates the state of the given button
     */
    void set_mouse_button_pressed(MouseButton button, bool pressed);

    /**
     * @brief Updates the state of the given key
     */
    void set_key_pressed(Key key, bool pressed);
private:
    EventManager* events_ = nullptr;
    MouseState mouse_;
    KeyboardState keyboard_;
};

#endif
