#include "scene.hxx"
#include "../entity/entity_event.hxx"
#include "../entity/move_action.hxx"
#include "../entity/entity_factory.hxx"
#include "../entity/components/render.hxx"
#include "../entity/components/vision.hxx"
#include "../gfx/renderer.hxx"
#include "tile_builder.hxx"
#include "entity/action_creator.hxx"

void Scene::init(EventManager* events, ServiceLocator* services) {
    events_ = events;
    services_ = services;
    assert(events_);
    assert(services_);

    tiles_.resize({ 120, 40 });
    for (auto& t : tiles_.cells()) {
        t = TileBuilder::floor();
    }

    events_->on<KeyDownEvent>(&Scene::on_key_down);
}

void Scene::shutdown() {
}

Entity* Scene::entity(u64 id) {
    auto it = entity_id_to_index_.find(id);
    if (it == entity_id_to_index_.end()) {
        return nullptr;
    }
    if (it->second >= entities_.size()) {
        return nullptr;
    }
    return entities_[it->second].get();
}

Entity& Scene::create_entity(const Archetype& archetype) {
    entities_.push_back(EntityFactory::create(archetype));
    auto& entity = entities_.back();

    entity_id_to_index_[entity->id] = entities_.size() - 1;
    return *entity.get();
}

Scene::EntityContainer& Scene::entities() {
    return entities_;
}

const Scene::EntityContainer& Scene::read_entities() {
    return entities_;
}

Grid<Tile>& Scene::tiles() {
    return tiles_;
}

const Grid<Tile>& Scene::read_tiles() {
    return tiles_;
}

void Scene::update() {
    if (!player_id_ || !player_action_) {
        return;
    }
    tiles_.each([](Tile& tile) { tile.fov = false; });

    for (auto& entity : entities_) {
        entity->update(player_action_->cost());
    }

    events_->trigger<EntitiesUpdated>();

    for (auto& entity : entities_) {
        entity->on_after_actions();
    }

    player_action_ = nullptr;
}

void Scene::draw() {
    auto& world_layer = Renderer::display();
    const auto& last_tile_index { world_layer.cell_count() };
    const auto scene_dim { tiles_.dimensions() };
    const auto display_dim { world_layer.dimensions() };
    const Vec2<u32> tile_render_dim (
        std::min<u32>(scene_dim.x, display_dim.x),
        std::min<u32>(scene_dim.y, display_dim.y)
    );

    for (u32 y = 0; y < tile_render_dim.y; ++y) {
        for (u32 x = 0; x < tile_render_dim.x; ++x) {
            const Vec2<u32> tile_pos { x, y };
            const Tile* tile { tiles_.at(tile_pos) };
            
            if (!tile->revealed) {
                continue;
            }

            auto cell = tile->display_info;

            // logic does not work with mutliple layers (game + ui)
            // the ui will be "burned in"
            // TODO: draw world and ui into separate buffers and then merge them

            if (!tile->fov) {
                world_layer.at(tile_pos)->color = Color::mono(128);
            } else {
                world_layer.put(std::move(cell), tile_pos);
            }
        }
    }

    for (const auto& entity : Scene::read_entities()) {
        if (!world_layer.in_bounds(entity->position)) {
            continue;
        }

        Render* render_component = entity->component<Render>();
        if (!render_component) {
            continue;
        }
        const auto& info = render_component->display_info();
        if (!info.visible) {
            continue;
        }

        const auto tile { tiles_.at(entity->position) };
        if (!tile || !tile->fov) {
            continue;
        }
        world_layer.put(DisplayCell(info.glyph, info.color), entity->position);
    }
}

void Scene::set_player(u64 id) {
    Entity* next_player = entity(id);
    if (next_player == nullptr) {
        Log::error("Cannot set entity {} as player: not found", id);
        return;
    }

    player_id_ = id;

    Entity* current_player = player();
    if (current_player) {
        current_player->on_player_detached();
    }

    next_player->energy = 0;
    next_player->on_player_attached();
}

Entity* Scene::player() {
    return entity(player_id_);
}

u64 Scene::player_id() {
    return player_id_;
}

bool Scene::on_key_down(KeyDownEvent& e) {
    Entity* player = Scene::player();
    if (!player) {
        return false;
    }
    auto move_relative = [=](Vec2<i32> direction) {
        player->energy = 99999;
        auto res = services_->get<IActionCreator>()->create_action(ActionType::Move, *player);
        if (res.action != nullptr) {
             static_cast<MoveAction*>(res.action)->destination = player->position + direction;
             player_action_ = res.action;
        }
    };
    switch (e.key) {
        case Key::W:
            move_relative(Vec2<i32> { 0, -1 });
            return true;
        case Key::A:
            move_relative(Vec2<i32> { -1, 0 });
            return true;
        case Key::S:
            move_relative(Vec2<i32> { 0, 1 });
            return true;
        case Key::D:
            move_relative(Vec2<i32> { 1, 0 });
            return true;
        default:
            return false;
    }
}

void Scene::apply_fov(const Vec2<i32> entity_pos, const Fov& fov) {
    const auto fov_dim = fov.vision_spots().dimensions();
    const auto fov_center = fov.center();

    for (i32 y = 0; y < fov_dim.y; ++y) {
        for (i32 x = 0; x < fov_dim.x; ++x) {
            const Vec2<i32> fov_pos { x, y };
            const VisionSpot* spot = fov.vision_spots().at(fov_pos);
            if (!spot) { // shouldnt happen?
                continue;
            }

            const Vec2<i32> tile_pos = entity_pos + fov_pos - fov_center;
            Tile *tile = tiles_.at(tile_pos);

            if (tile) {
                // do not black out tiles in multiple overlappin fovs
                // alternative: if (!tile->fov) tile->fov = spot->visible;
                tile->fov |= !tile->fov & spot->visible;
            }
        }
    }
}
