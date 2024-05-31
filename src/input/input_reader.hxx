#ifndef JARALYN_INPUT_READER_HXX
#define JARALYN_INPUT_READER_HXX

enum class MouseButton;
enum class Key;

class IInputReader {
public:
    /**
     * @brief Returns the current mouse position as a tuple
     */
    virtual Vec2<u32> mouse_position() = 0;

    /**
     * @brief Returns the current x coordinate of the mouse cursor
     */
    virtual u32 mouse_x() const = 0;

    /**
     * @brief Returns the current y coordinate of the mouse cursor
     */
    virtual u32 mouse_y() const = 0;

    /**
     * @brief Returns true if the given mouse button is currently pressed
     */
    virtual bool is_mouse_pressed(MouseButton button) const = 0;

    /**
     * @brief Returns true if the given keyboard key is currently pressed
     */
    virtual bool is_key_pressed(Key key) const = 0;
};

#endif
