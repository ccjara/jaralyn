#include "component/vision/fov.hxx"

void Fov::update(Entity& viewer, Grid<Tile>& tiles) {
    // from the viewer position, map solid tiles to the vision spots
    // then we scan the tiles in the four quadrants and perform the shadowcasting
    const auto viewer_pos = viewer.position;
    const auto vision_spots_dimensions = vision_spots_.dimensions();
    const auto vision_center = center();

    if (vision_spots_.byte_size() == 0) {
        return;
    }

    for (i32 y = 0; y < vision_spots_dimensions.y; ++y) {
        for (i32 x = 0; x < vision_spots_dimensions.x; ++x) {
            const auto spot_pos = Vec2<i32>{ x, y };
            VisionSpot* vision_spot = vision_spots_.at(spot_pos);
            if (!vision_spot) {
                Log::error("Fov::update: vision spot at {}, {} is null", spot_pos.x, spot_pos.y);
                continue; // shouldnt happen?
            }
            vision_spot->visible = false;

            if (vision_spot->blind) {
                continue;
            }

            const auto tile_pos = viewer_pos + spot_pos - vision_center;
            const auto tile = tiles.at(tile_pos);

            if (tile) {
                vision_spot->blocker = tile->solid;
            }
        }
    }

    vision_spots_.at(vision_center)->visible = true;

    for (i32 q = 0; q < 4; ++q) {
        Row row(1, -1.0f, 1.0f);
        scan(viewer, row, (Quadrant) q);
    }
}

void Fov::set_vision_radius(i32 radius) {
    vision_radius_ = std::max(0, radius);
    const u32 size = 2 * vision_radius_ + 1;
    vision_spots_.resize({ size, size });
    apply_blind_spots();
}

void Fov::apply_blind_spots() {
    const auto center_pos = center();
    const auto dim = vision_spots_.dimensions();

    for (i32 y = 0; y < dim.y; ++y) {
        for (i32 x = 0; x < dim.x; ++x) {
            const Vec2<i32> spot_pos { x, y };
            VisionSpot* vision_spot = vision_spots_.at(spot_pos);
            if (!vision_spot) {
                continue;
            }
            const auto offset = spot_pos - center_pos;
            // not sure if we can achieve the chopped-off corner look without sqrt
            // but this calculation is only invoked on vision radius update which is rare
            vision_spot->blind = (0.5f + std::sqrt(offset.x * offset.x + offset.y * offset.y)) > (vision_radius_ + 1);
        }
    }
}

i32 Fov::vision_radius() const {
    return vision_radius_;
}

Vec2<i32> Fov::center() const {
    return Vec2<i32>{ vision_radius_, vision_radius_ };
}

const Grid<VisionSpot>& Fov::vision_spots() const {
    return vision_spots_;
}

void Fov::scan(
    Entity& viewer,
    Row& row,
    Quadrant q
) {
    VisionSpot *prev_spot { nullptr };

    const auto center = this->center();

    for (i32 col = row.min_col; col <= row.max_col; ++col) {
        const auto grid_pos = to_grid_coords(q, center, row.depth, col);
        VisionSpot* spot = vision_spots_.at(grid_pos);
        if (!spot || spot->blind) {
            continue;
        }

        if (spot->blocker || symmetric(row, col)) {
            spot->visible = true;
        }
        if (prev_spot) {
            if (prev_spot->blocker && !spot->blocker) {
                row.set_start_slope(slope(row.depth, col));
            }
            if (!prev_spot->blocker && spot->blocker) {
                Row next = row.next();
                next.set_end_slope(slope(row.depth, col));
                scan(viewer, next, q);
            }
        }
        prev_spot = spot;
    }
    if (prev_spot && !prev_spot->blocker && row.depth < vision_radius_) {
        auto next = row.next();
        scan(viewer, next, q);
    }
}

float Fov::slope(i32 depth, i32 column) {
    return ((float) (2 * column - 1)) / ((float) 2 * depth);
}

bool Fov::symmetric(Row& row, i32 col) {
    return ((float) col >= (float) row.depth * row.start_slope) &&
            ((float) col <= (float) row.depth * row.end_slope);
}

Vec2<i32> Fov::to_grid_coords(Quadrant q, Vec2<i32> start_pos, i32 depth, i32 col) {
    switch (q) {
        case Quadrant::N:
        default:
            return { start_pos.x + col, start_pos.y - depth };
        case Quadrant::S:
            return { start_pos.x + col, start_pos.y + depth };
        case Quadrant::E:
            return { start_pos.x + depth, start_pos.y + col };
        case Quadrant::W:
            return { start_pos.x - depth, start_pos.y + col };
    }
}
