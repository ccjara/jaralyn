#ifndef JARALYN_FOV_HXX
#define JARALYN_FOV_HXX

#include "tile/tile.hxx"
#include "entity/entity.hxx"

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

struct VisionSpot {
    /**
     * @brief Whether the spot is visible (in range and not blocked by obstacles)
     */
    bool visible;

    /**
     * @brief Whether this spot is a blocker (for example a wall tile)
     */
    bool blocker;

    /**
     * @brief Whether this spot is a blind spot (due to oval vision radius)
     */
    bool blind;
};

/**
 * @brief Manages vision
 *
 * Algorithm based on https://www.albertford.com/shadowcasting
 */
class Fov {
public:
    void update(Entity& viewer, Grid<Tile>& tiles);

    /**
     * @brief Sets the vision radius, resetting the fov
     */
    void set_vision_radius(i32 radius);

    /**
     * @brief Returns the current vision radius
     */
    i32 vision_radius() const;

    /**
     * @brief Returns the center of the vision spots grid
     */
    Vec2<i32> center() const;

    /**
     * @brief Provides readonly access to the vision spots grid
     */
    const Grid<VisionSpot>& vision_spots() const;
private:

    /**
     * @brief The grid of vision spots, representing the field of view
     * 
     * The player is always the center of the grid
     * 
     * Example: vision_radius_ = 1,
     * ...
     * .@.
     * ...
     * 
     * Example: vision_radius_ = 2,
     * .....
     * .....
     * ..@..
     * .....
     * .....
     */
    Grid<VisionSpot> vision_spots_;

    int vision_radius_;

    enum class Quadrant { N, E, S, W };

    inline void scan(Entity& viewer, Row& row, Quadrant q);

    inline Vec2<i32> static to_grid_coords(Quadrant q, Vec2<i32> start_pos, i32 depth, i32 col);

    inline void apply_blind_spots();

    static inline float slope(i32 depth, i32 column);

    static inline bool symmetric(Row& row, i32 col);
};

#endif
