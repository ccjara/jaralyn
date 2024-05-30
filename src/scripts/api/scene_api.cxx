#include "scene_api.hxx"
#include "entity/entity.hxx"
#include "entity/entity_reader.hxx"

SceneApi::SceneApi(IEntityReader* entity_reader) : entity_reader_(entity_reader) {
    assert(entity_reader_);
}

void SceneApi::on_register(Script* script) {
    luabridge::getGlobalNamespace(*script)
        .beginClass<SceneApi>("SceneApi")
            .addFunction("entity_name", &SceneApi::entity_name)
            .addFunction("player_id", &SceneApi::player_id)
            .addFunction("player_name", &SceneApi::player_name)
        .endClass();

    expose(script, this, "scene");
}

const char* SceneApi::entity_name(u64 id) const {
    Entity* entity = entity_reader_->entity(id);
    if (!entity) {
        return nullptr;
    }
    return entity->name.c_str();
}

u64 SceneApi::player_id() const {
    Entity* player = entity_reader_->player();
    if (!player) {
        return null_id;
    }
    return player->id;
}

const char* SceneApi::player_name() const {
    Entity* player = entity_reader_->player();
    if (!player) {
        return nullptr;
    }
    return player->name.c_str();
}
