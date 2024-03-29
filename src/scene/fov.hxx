#ifndef JARALYN_FOV_HXX
#define JARALYN_FOV_HXX

#include "../grid.hxx"
#include "../scene/tile.hxx"
#include "../entity/entity.hxx"

namespace {
    struct Row {
        i32 depth;
        float start_slope;
        float end_slope;
        i32 min_col;
        i32 max_col;

        Row(i32 depth, float start_slope, float end_slope) :
            depth(depth) {
            set_start_slope(start_slope);
            set_end_slope(end_slope);
        }

        void set_start_slope(float slope) {
            start_slope = slope;
            min_col = std::round(((float) depth) * slope);
        }

        void set_end_slope(float slope) {
            end_slope = slope;
            max_col = std::round(((float) depth) * slope);
        }

        Row next() const {
            return { depth + 1, start_slope, end_slope };
        }
    };
}

/**
 * @brief Manages vision
 *
 * Algorithm based on https://www.albertford.com/shadowcasting
 */
class Fov {
public:
    static void update(Entity& viewer, Grid<Tile>& tiles);
private:
    enum class Quadrant { N, E, S, W };

    static inline void scan(Entity& viewer, Grid<Tile>& tiles, Row& row, Quadrant q);

    inline Vec2<i32> static to_grid_coords(Quadrant q, Vec2<i32> start_pos, i32 depth, i32 col);

    static inline float slope(i32 depth, i32 column);

    static inline bool symmetric(Row& row, i32 col);

    static inline bool in_range(i32 row, i32 col, i32 max_range);
};

#endif
