#ifndef NOEDEN_COLOR_HXX
#define NOEDEN_COLOR_HXX

/**
 * @brief Represents a color which can be displayed by the gfx display
 *
 * This class is globally available as part of the pch.
 */
struct Color {
    u8 r = 255;
    u8 g = 255;
    u8 b = 255;

    constexpr Color() = default;

    explicit constexpr Color(i32 value) {
        assign_from_int(value);
    }

    constexpr Color(u8 r, u8 g, u8 b) :
        r(r), g(g), b(b) {
    }

    constexpr Color& operator=(i32 value) {
        assign_from_int(value);
        return *this;
    }

    static constexpr Color mono(u8 value) {
        return Color(value, value, value);
    }

    static constexpr Color white() {
        return Color::mono(255);
    }

    static constexpr Color green() {
        return Color(0, 255, 0);
    }

    static constexpr Color red() {
        return Color(255, 0, 0);
    }

    static constexpr Color yellow() {
        return Color(255, 255, 0);
    }

    static constexpr Color blue() {
        return Color(0, 0, 255);
    }

    static constexpr Color black() {
        return Color(0, 0, 0);
    }

    static constexpr Color from_string(std::string_view text) {
        i32 hex_color { 0xFFFFFF };
        std::from_chars(text.data(), text.data() + text.size(), hex_color, 16);
        return Color(hex_color);
    }

    constexpr bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b;
    }
private:
    constexpr void assign_from_int(i32 value) {
        r = static_cast<u8> ((value >> 16) & 0xFF);
        g = static_cast<u8> ((value >> 8) & 0xFF);
        b = static_cast<u8> (value & 0xFF);
    }
};

#endif
