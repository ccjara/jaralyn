#include <catch2/catch_test_macros.hpp>
#include "math/vec2.hxx"

TEST_CASE("Vec2::Add adds numbers", "[math][unit]") {
    Vec2<int> vec(1, 2);

    vec += Vec2<int>(3, 4);

    REQUIRE(vec.x == 4);
    REQUIRE(vec.y == 6);
}
