#include "scene.hxx"

void Scene::init() {
    tiles_.resize({ 120, 40 });
    for (auto& t : tiles_.cells()) {
        t = TileBuilder::floor();
    }

    Events::on<KeyDownEvent>(&Scene::on_key_down);
}

void Scene::shutdown() {
}

Entity* Scene::get_entity_by_id(u64 id) {
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

void Scene::update_fov(u64 id) {
    Entity* entity = get_entity_by_id(id);
    if (!entity) {
        return;
    }
    Fov::update(*entity, tiles_);
}

void Scene::perform_actions() {
    if (!player_id_ || !player_action_) {
        return;
    }
    for (auto& entity : entities_) {
        entity->update(player_action_->cost);
    }
    std::sort(
        actions_.begin(),
        actions_.end(),
        [](const auto& a, const auto& b) {
            return a->speed > b->speed;
        }
    );
    for (auto& action : actions_) {
        action->perform();
    }
    actions_.clear();

    update_fov(player_id_);
    player_action_ = nullptr;
}

void Scene::set_player(u64 id) {
    player_id_ = id;
}

Entity* Scene::player() {
    return get_entity_by_id(player_id_);
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
        player_action_ = &create_action<MoveAction>(player, player->position + direction);
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
        case Key::I: { // TEST (Needs layer stack)
            static bool show = true;
            Events::trigger<InventoryEvent>(show ? player->id : null_id);
            show = !show;
            return true;
        }
        default:
            return false;
    }
}