#include "world_pos.hxx"

WorldPos::WorldPos() : x(0), y(0), z(0) {}

WorldPos::WorldPos(i32 x, i32 y, i32 z) : x(x), y(y), z(z) {}

bool WorldPos::operator==(const WorldPos& other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool WorldPos::operator!=(const WorldPos& other) const {
    return !(*this == other);
}

WorldPos WorldPos::operator+(const WorldPos& other) const {
    return WorldPos(x + other.x, y + other.y, z + other.z);
}

WorldPos WorldPos::operator-(const WorldPos& other) const {
    return WorldPos(x - other.x, y - other.y, z - other.z);
}

WorldPos WorldPos::operator*(i32 scalar) const {
    return WorldPos(x * scalar, y * scalar, z * scalar);
}

WorldPos WorldPos::operator/(i32 scalar) const {
    return WorldPos(x / scalar, y / scalar, z / scalar);
}

WorldPos& WorldPos::operator+=(const WorldPos& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

WorldPos& WorldPos::operator-=(const WorldPos& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

WorldPos& WorldPos::operator*=(i32 scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

WorldPos& WorldPos::operator/=(i32 scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

double WorldPos::distance_to(const WorldPos& other) const {
    return std::sqrt((x - other.x) * (x - other.x) +
                     (y - other.y) * (y - other.y) +
                     (z - other.z) * (z - other.z));
}

i32 WorldPos::manhattan_distance_to(const WorldPos& other) const {
    return std::abs(x - other.x) + std::abs(y - other.y) + std::abs(z - other.z);
}

std::string WorldPos::to_string() const {
    return fmt::format("({},{},{})", x, y, z);
}
