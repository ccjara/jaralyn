#ifndef JARALYN_KEYBOARD_STATE_HXX
#define JARALYN_KEYBOARD_STATE_HXX

#include "input/key.hxx"

class KeyboardState {
public:
    /**
     * @brief Returns true if the given key is pressed.
     */
    bool is_pressed(Key key) const;

    /**
     * @brief Sets the given key state to `pressed`.
     */
    void key_down(Key key);

    /**
     * @brief Sets the given key state to `released`.
     */
    void key_up(Key key);
private:
    std::unordered_map<Key, bool> keys_;
};


#endif
