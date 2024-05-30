#ifndef JARALYN_DISPLAY_HXX
#define JARALYN_DISPLAY_HXX

#include "text.hxx"

struct TextState {
    Color color { Color::white() };
    bool break_word { true };
};

struct RectOptions {
    /**
     * @brief Rectangle span (coordinates)
     */
    Rect<i32> span;

    /**
     * @brief Border color to use
     */
    Color color;

    /**
     * @brief Optional fill color to use (transparent, if omitted)
     */
    std::optional<Color> fill_color { std::nullopt };

    /**
     * @brief Glyphs to use when rendering its sides
     */
    Rect<u32> border_glyphs { 745U, 745U, 745U, 745U };

    /**
     * @brief Glyphs to use when rendering its corners
     */
    RectCorners<u32> corner_glyphs {
        745U,
        745U,
        745U,
        745U
    };
};

/**
 * @brief A grid comprised of cells representing a single character
 *
 * On each frame the current display state is copied into the text shader
 * which handles the rendering of each individual character.
 *
 * @see TextShader
 */
class Display: public Grid<DisplayCell> {
private:
    constexpr static unsigned char CONTROL_CHAR { '$' };

    constexpr static size_t MAX_STATES { 128 };
    std::array<TextState, MAX_STATES> states_;
    TextState* state_ { states_.data() };
    TextState* const first_state_ { &states_.front() };
    TextState* const last_state_ { &states_.back() };

    /**
     * @brief Attempts to parse a color from the given string. Returns white on failure.
     */
    inline Color parse_color(std::string_view text) const;

    /**
     * @brief Copies the current state into the subsequent state, updating the state pointer accordingly
     */
    inline void push_copy();
public:
    /**
     * @brief Render a text to the display
     *
     * If a clamp coordinate is set to the numeric maximum the renderer will
     * constrain the text to the display's width or height respectively.
     * See below for more information about break modes.
     *
     * Text is formattable using the control character $. When encountering it,
     * the text renderer will apply formatting rules based on the desired operation.
     *
     * Depending on the operation, the text renderer may push a new formatting
     * state on its state stack. This way text renderer 'remembers' what format
     * was used when the new formatting is undone.
     *
     * $c renders all subsequent characters in the given color (hex).
     *    usage: $cRRGGBB (RRGGBB as hex)
     * $! pops the current formatting state from the state stack, restoring the
     *    previously active state. if the stack is empty, this will be a noop
     * $n breaks the current line (does not modify the state stack)
     *    usage: Hello $n World
     * $w set text break mode to `break word` (default) which renders clipping
     *    words on the next line (unless the next line is vertically clipping)
     * $a set text break mode to `break all` which immediately renders a clipping
     *    character on the next line, regardless if it is mid-word or not
     */
    void text(
        std::string_view t,
        Vec2<u32> position,
        Vec2<u32> clamp = {
            std::numeric_limits<u32>::max(),
            std::numeric_limits<u32>::max()
        }
    );

    void text(const Text& t, Vec2<i32> position);

    /**
     * @brief Render a rectangle on the display
     */
    void rectangle(const RectOptions& options);

    /**
     * @brief Render a line on the display using bresenham's algorithm
     */
    void line(Vec2<u32> from, Vec2<u32> to, u32 glyph = 750, Color color = Color());
};

#endif
