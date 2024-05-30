#include "component/vision/vision.hxx"
#include "tile/tile_reader.hxx"

Vision::Vision(ITileReader* tile_reader) : tile_reader_(tile_reader) {
    assert(tile_reader_);
}

void Vision::update(u64 dt) {
}

void Vision::set_vision_radius(i32 radius) {
    fov_.set_vision_radius(std::max(radius, 1));
}

i32 Vision::vision_radius() const {
    return fov_.vision_radius();
}

const Fov& Vision::fov() const {
    return fov_;
}

void Vision::on_after_actions() {
    update_fov();
}

bool Vision::applied() const {
    return apply_;
}

void Vision::set_apply(bool apply_vision) {
    apply_ = apply_vision;
}

void Vision::on_player_attached() {
    apply_ = true;
    update_fov();
}

void Vision::on_player_detached() {
    apply_ = false;
}

void Vision::update_fov() {
    if (entity_ == nullptr) {
        return;
    }
    fov_.update(*entity_, tile_reader_->tiles());

    if (!apply_) {
        return;
    }
    // TODO: temporary code
    const auto fov_dim = fov_.vision_spots().dimensions();
    const auto fov_center = fov_.center();
    const auto entity_pos = entity_->position;
    Grid<Tile>& tiles = tile_reader_->tiles();

    for (i32 y = 0; y < fov_dim.y; ++y) {
        for (i32 x = 0; x < fov_dim.x; ++x) {
            const Vec2<i32> fov_pos { x, y };
            const VisionSpot* spot = fov_.vision_spots().at(fov_pos);
            if (!spot) { // shouldnt happen?
                continue;
            }

            const Vec2<i32> tile_pos = entity_pos + fov_pos - fov_center;
            Tile *tile = tiles.at(tile_pos);

            if (tile) {
                // do not black out tiles in multiple overlapping fovs
                // alternative: if (!tile->fov) tile->fov = spot->visible;
                tile->fov |= !tile->fov & spot->visible;
            }
        }
    }
}

