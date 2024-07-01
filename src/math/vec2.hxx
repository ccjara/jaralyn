#ifndef NOEDEN_VEC2_HXX
#define NOEDEN_VEC2_HXX

template<typename t>
struct Vec2 {
    using type = t;

    t x { 0 };
    t y { 0 };

    /**
     * @brief Constructs a null-vector
     */
    constexpr Vec2() = default;

    /**
     * @brief Copy-constructs a vector from another vector
     */
    constexpr Vec2(const Vec2<t>& v) {
        x = v.x;
        y = v.y;
    }

    /**
     * @brief Copy-constructs a vector from another vector
     */
    template<typename u>
    constexpr Vec2(const u& v) {
        x = static_cast<t> (v.x);
        y = static_cast<t> (v.y);
    }

    /**
     * @brief Constructs a vector from two scalars
     */
    constexpr Vec2(t x, t y) {
        this->x = x;
        this->y = y;
    }

    /**
     * @brief Adds the given vector
     */
    constexpr void operator+=(const Vec2<t>& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
    }

    /**
     * @brief Adds the given scalar
     */
    constexpr void operator+=(const t scalar) {
        this->x += scalar;
        this->y += scalar;
    }

    /**
     * @brief Subtracts the given vector
     */
    constexpr void operator-=(const Vec2<t>& rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
    }

    /**
     * @brief Subtracts the given scalar
     */
    constexpr void operator-=(const t scalar) {
        this->x -= scalar;
        this->y -= scalar;
    }

    /**
     * @brief Multiplies the vector parts by the given scalar
     */
    constexpr void operator*=(const t scalar) {
        this->x *= scalar;
        this->y *= scalar;
    }

    /**
     * @brief Divides the vector parts by the given scalar
     */
    constexpr void operator/=(const t scalar) {
        this->x /= scalar;
        this->y /= scalar;
    }

    /**
     * @brief Computes the manhattan distance between two vectors (sum of absolute differences of their coordinates)
     */
    [[nodiscard]] constexpr t manhattan_distance(const Vec2<t>& v) const {
        return std::abs(v.x - x) + std::abs(v.y - y);
    }
};

/**
 * @brief Compares two vectors for equality
 *
 * Unreliable on floats
 */
template<typename t>
[[nodiscard]] constexpr bool operator==(const Vec2<t>& lhs, const Vec2<t>& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

/**
 * @brief Compares two vectors for inequality
 */
template<typename t>
[[nodiscard]] constexpr bool operator!=(const Vec2<t>& lhs, const Vec2<t>& rhs) {
    return !(lhs == rhs);
}

/**
 * @brief Returns a new negated version of the given vector
 */
template<typename t>
[[nodiscard]] constexpr Vec2<t> operator-(const Vec2<t>& v) {
    return Vec2<t>(-v.x, -v.y);
}

/**
 * @brief Subtracts a scalar from all vector parts
 */
template<typename t>
[[nodiscard]] constexpr Vec2<t> operator-(const Vec2<t>& v, t scalar) {
    return Vec2<t>(v.x - scalar, v.y - scalar);
}

/**
 * @brief Subtracts two given vectors
 */
template<typename t>
[[nodiscard]] constexpr Vec2<t> operator-(const Vec2<t>& lhs, const Vec2<t>& rhs) {
    return Vec2<t>(lhs.x - rhs.x, lhs.y - rhs.y);
}

/**
 * @brief Adds two given vectors
 */
template<typename t>
[[nodiscard]] constexpr Vec2<t> operator+(const Vec2<t>& lhs, const Vec2<t>& rhs) {
    return Vec2<t>(lhs.x + rhs.x, lhs.y + rhs.y);
}

/**
 * @brief Adds a scalar to all vector parts
 */
template<typename t>
[[nodiscard]] constexpr Vec2<t> operator+(const Vec2<t>& v, t scalar) {
    return Vec2<t>(v.x + scalar, v.y + scalar);
}

/**
 * @brief Divides all vector parts by the given scalar
 */
template<typename t>
[[nodiscard]] constexpr Vec2<t> operator/(const Vec2<t>& v, t scalar) {
    return Vec2<t>(v.x / scalar, v.y / scalar);
}

/**
 * @brief Multiplies all vector parts by the given scalar
 */
template<typename t>
[[nodiscard]] constexpr Vec2<t> operator*(const Vec2<t>& v, t scalar) {
    return Vec2<t>(v.x * scalar, v.y * scalar);
}

#endif
