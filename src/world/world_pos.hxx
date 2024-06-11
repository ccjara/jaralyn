#ifndef NOEDEN_WORLD_POS_HXX
#define NOEDEN_WORLD_POS_HXX

/**
 * @brief Coordinate in a Y-up world
 */
struct WorldPos {
    i32 x;
    i32 y;
    i32 z;

    WorldPos();

    WorldPos(i32 x, i32 y, i32 z);

    bool operator==(const WorldPos& other) const;

    bool operator!=(const WorldPos& other) const;

    WorldPos operator+(const WorldPos& other) const;

    WorldPos operator-(const WorldPos& other) const;

    WorldPos operator*(i32 scalar) const;

    WorldPos operator/(i32 scalar) const;

    WorldPos& operator+=(const WorldPos& other);

    WorldPos& operator-=(const WorldPos& other);

    WorldPos& operator*=(i32 scalar);

    WorldPos& operator/=(i32 scalar);

    double distance_to(const WorldPos& other) const;

    i32 manhattan_distance_to(const WorldPos& other) const;

    std::string to_string() const;
};

#endif