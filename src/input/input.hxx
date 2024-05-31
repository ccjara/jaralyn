#ifndef JARALYN_INPUT_HXX
#define JARALYN_INPUT_HXX

#include "mouse_state.hxx"
#include "keyboard_state.hxx"

/**
 * @brief Readonly input state provider
 */
class Input {
    friend class Platform;
public:
    /**
     * @brief Returns the current mouse position as a tuple
     */
    static Vec2<u32> mouse_position();

    /**
     * @brief Returns the current x coordinate of the mouse cursor
     */
    static u32 mouse_x();

    /**
     * @brief Returns the current y coordinate of the mouse cursor
     */
    static u32 mouse_y();

    /**
     * @brief Returns true if the given mouse button is currently pressed
     */
    static bool is_mouse_pressed(MouseButton button);

    /**
     * @brief Returns true if the given keyboard key is currently pressed
     */
    static bool is_key_pressed(Key key);
private:
    static inline MouseState mouse_;
    static inline KeyboardState keyboard_;
};

#endif
