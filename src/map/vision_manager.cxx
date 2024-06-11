#include "vision_manager.hxx"
#include "entity/entity.hxx"
#include "entity/entity_reader.hxx"
#include "component/vision/vision.hxx"
#include "tile/tile.hxx"
#include "tile/tile_reader.hxx"

VisionManager::VisionManager(
    IEntityReader* entity_reader,
    ITileReader* tile_reader,
    Events* events
) : entity_reader_(entity_reader), tile_reader_(tile_reader), events_(events) {
    assert(entity_reader_);
    assert(tile_reader_);
    assert(events_);

    world_ready_sub_ = events->engine->on<WorldReadyEvent>(this, &VisionManager::on_world_ready);
    world_updated_pre_sub_ = events->engine->on<WorldUpdatedPreEvent>(this, &VisionManager::on_world_updated_pre);
    world_updated_post_sub_ = events->engine->on<WorldUpdatedPostEvent>(this, &VisionManager::on_world_updated_post);
}

EventResult VisionManager::on_world_ready(const WorldReadyEvent&) {
    update_fov();
    apply_fov();
    return EventResult::Continue;
}

EventResult VisionManager::on_world_updated_pre(const WorldUpdatedPreEvent&) {
    // reset fov
    tile_reader_->tiles().each([](Tile& tile) {
        tile.flags.set(TileFlags::FoV, false);
    });

    return EventResult::Continue;
}

EventResult VisionManager::on_world_updated_post(const WorldUpdatedPostEvent&) {
    update_fov();
    apply_fov();
    return EventResult::Continue;
}

void VisionManager::apply_fov() {
    /* TODO
    for (auto& entity: entity_reader_->entities()) {
        auto vision = entity->component<Vision>();
        if (vision == nullptr) {
            continue;
        }
        if (!vision->applied()) {
            continue;
        }

        // REFACTOR: simplify and reuse apply_fov / update fov

        const auto vision_spots = vision->fov().vision_spots();
        const auto vision_spots_dim = vision_spots.dimensions();
        const auto offset = entity->position - vision->fov().center();
        auto spot_pos = Vec2<i32>{ 0, 0 };

        for (spot_pos.y = 0; spot_pos.y < vision_spots_dim.y; ++spot_pos.y) {
            for (spot_pos.x = 0; spot_pos.x < vision_spots_dim.x; ++spot_pos.x) {
                const auto& spot = vision_spots.at(spot_pos);
                if (spot == nullptr || spot->blind) {
                    continue;
                }
                const auto tile_pos = spot_pos + offset;
                auto tile = tile_reader_->tile(tile_pos);
                if (tile == nullptr) {
                    continue;
                }

                if (tile->flags.test(TileFlags::FoV)) {
                    tile->flags.set(TileFlags::Revealed);
                } else {
                    tile->flags.set(TileFlags::FoV, spot->visible);
                }
            }
        }
    }
    */
}

void VisionManager::update_fov() {
    /* TODO
    for (auto& entity: entity_reader_->entities()) {
        auto vision = entity->component<Vision>();
        if (vision == nullptr) {
            return;
        }
        auto& fov = vision->fov();
        auto& spots = fov.vision_spots();
        const auto spots_dim = spots.dimensions();
        const auto vision_center = fov.center();
        const auto entity_pos = entity->position;
        Grid<Tile>& tiles = tile_reader_->tiles();

        auto spot_pos = Vec2<i32>{ 0, 0 };

        for (spot_pos.y = 0; spot_pos.y < spots_dim.y; ++spot_pos.y) {
            for (spot_pos.x = 0; spot_pos.x < spots_dim.x; ++spot_pos.x) {
                VisionSpot* vision_spot = spots.at(spot_pos);
                if (!vision_spot) {
                    continue;
                }
                vision_spot->visible = false;

                if (vision_spot->blind) {
                    continue;
                }

                const auto tile_pos = entity_pos + spot_pos - vision_center;
                const auto tile = tiles.at(tile_pos);

                if (!tile) {
                    continue;
                }
                vision_spot->blocker = tile->flags.test(TileFlags::Blocking);
            }
        }
        fov.update();
    }
    */
}