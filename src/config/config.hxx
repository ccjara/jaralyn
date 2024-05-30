#ifndef JARALYN_CONFIG_HXX
#define JARALYN_CONFIG_HXX

/**
 * @brief Contains platform / system options for the game
 */
struct Config {
    /**
     * @brief Size in pixels of each individual character
     * 
     * The size depends on the font texture and must be adjusted accordingly.
     */
    Vec2<u32> glyph_size { 8, 14 };

    /**
     * @brief Multiplies the size of each rendered character
     */
    u32 scaling { 1 };

    /**
     * Path to load the bitmap font texture from (currently only bmp is supported)
     */
    fs::path font_texture_path { "font.bmp" };
};

#endif
