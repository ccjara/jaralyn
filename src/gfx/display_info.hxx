#ifndef JARALYN_DISPLAY_INFO_HXX
#define JARALYN_DISPLAY_INFO_HXX

/**
 * @brief Display drawing instructions
 */
struct DisplayInfo {
    /**
     * @brief The glyph to render
     */
    u32 glyph = 0U;

    /**
     * @brief The color to use when rendering the glyph.
     */
    Color color;

    /**
     * @brief Skips drawing this glyph if false.
     */
    bool visible = false;
};

#endif
