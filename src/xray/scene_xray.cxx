#include "scene_xray.hxx"
#include "entity/entity.hxx"
#include "component/vision/vision.hxx"
#include "component/skills.hxx"
#include "component/render.hxx"
#include "input/input.hxx"
#include "input/input_event.hxx"
#include "gfx/renderer.hxx"
#include "component/skills.hxx"
#include "entity/entity_manager.hxx"
#include "tile/tile_manager.hxx"
#include "config/config_event.hxx"

SceneXray::SceneXray(
    EntityManager* entity_manager,
    TileManager* tile_manager,
    EventManager* events,
    IInputReader* input
) : entity_manager_(entity_manager), 
    tile_manager_(tile_manager),
    events_(events),
    input_(input) {
    assert(entity_manager_);
    assert(tile_manager_);
    assert(input_);
    assert(events);

    events_->on<MouseDownEvent>(this, &SceneXray::on_mouse_down, 9000);
    events_->on<ConfigUpdatedEvent>(this, &SceneXray::on_config_updated, 9000);
}

bool SceneXray::on_config_updated(ConfigUpdatedEvent& e) {
    config_ = e.next;
    return false;
}

bool SceneXray::on_mouse_down(MouseDownEvent& e) {
    if (!tile_window_data_.is_editing) {
        return false;
    }

    TileType type_to_place;
    if (input_->is_mouse_pressed(MouseButton::Left)) {
        type_to_place = tile_window_data_.lmb_type;
    } else if (input_->is_mouse_pressed(MouseButton::Right)) {
        type_to_place = tile_window_data_.rmb_type;
    } else {
        return false;
    }
    const auto mp = input_->mouse_position();
    const Vec2<u32> tpos = {
        mp.x / (config_.glyph_size.x * config_.scaling),
        mp.y / (config_.glyph_size.y * config_.scaling)
    };
    if (!tile_manager_->tiles().at(tpos)) {
        return false;
    }
    auto tile = tile_manager_->create_tile(type_to_place, tpos);
    tile.revealed = true;
    tile_manager_->tiles().put(std::move(tile), tpos);
    return true;
}

void SceneXray::update() {
    entity_window();
    tile_window();
}

void SceneXray::entity_window() {
    static std::optional<u64> entity_id = std::nullopt;
    static std::string combo_label = "Select entity";

    ImGui::Begin("Entities");

    if (ImGui::BeginCombo("Entity", combo_label.c_str())) {
        for (auto& entity : entity_manager_->entities()) {
            const auto& entity_name_str = entity->name.c_str();
            const bool is_selected = entity_id.has_value() && entity_id.value() == entity->id;
            if (ImGui::Selectable(entity_name_str, is_selected)) {
                entity_id = entity->id;
                combo_label = entity_name_str;
            }
        }
        ImGui::EndCombo();
    }
    entity_panel(entity_id);

    ImGui::End();
}

void SceneXray::tile_window() {
    const static std::unordered_map<TileType, std::string> type_options = {
        { TileType::None, "None" },
        { TileType::Floor, "Floor" },
        { TileType::Wall, "Wall" },
        { TileType::Water, "Water" },
    };

    ImGui::Begin("Tiles");
    ImGui::PushItemWidth(ImGui::GetWindowWidth() / 4);
    if (ImGui::BeginCombo("LMB Tile", type_options.at(tile_window_data_.lmb_type).c_str())) {
        for (auto& option : type_options) {
            const bool is_selected = tile_window_data_.lmb_type == option.first;
            if (ImGui::Selectable(option.second.c_str(), is_selected)) {
                tile_window_data_.lmb_type = option.first;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();

    if (ImGui::BeginCombo("RMB Tile", type_options.at(tile_window_data_.rmb_type).c_str())) {
        for (auto& option : type_options) {
            const bool is_selected = tile_window_data_.rmb_type == option.first;
            if (ImGui::Selectable(option.second.c_str(), is_selected)) {
                tile_window_data_.rmb_type = option.first;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth(); // select width
    ImGui::Checkbox("Edit Mode", &tile_window_data_.is_editing);
    ImGui::End();
}

void SceneXray::entity_panel(std::optional<u64> entity_id) {
    if (!entity_id.has_value()) {
        return;
    }
    Entity* entity = entity_manager_->entity(entity_id.value());
    if (entity == nullptr) {
        return;
    }
    entity_glyph(entity);

    i32 position_raw[2] = { entity->position.x, entity->position.y };
    bool is_player = entity_manager_->player() == entity;

    ImGui::Text("Id: %lx", entity->id);
    if (ImGui::Checkbox("Player", &is_player)) {
        entity_manager_->set_controlled_entity(is_player ? entity->id : null_id);
    }
    ImGui::PushItemWidth(ImGui::GetWindowWidth() / 4);
    if (ImGui::InputInt2("Position", position_raw, ImGuiInputTextFlags_None)) {
        position_raw[0] = std::min(std::max(position_raw[0], 0), 100);
        position_raw[1] = std::min(std::max(position_raw[1], 0), 100);
        entity->position.x = position_raw[0];
        entity->position.y = position_raw[1];
    }
    if (ImGui::InputInt("Speed", &entity->speed, ImGuiInputTextFlags_None)) {
        entity->speed = std::max(entity->speed, 0);
    }
    if (ImGui::InputInt("Energy", &entity->energy, ImGuiInputTextFlags_None)) {
        entity->energy = std::max(entity->energy, 0);
    }

    // TODO: method on component? like component->draw_xray();
    Vision* vision_component = entity->component<Vision>();
    if (vision_component != nullptr) {
        auto radius = vision_component->vision_radius();
        if (ImGui::InputInt("Vision Radius", &radius, ImGuiInputTextFlags_None)) {
            vision_component->set_vision_radius(radius);
        }
        bool apply = vision_component->applied();
        if (ImGui::Checkbox("Vision Applied", &apply)) {
            vision_component->set_apply(apply);
        }
    }

    Skills* skills_component = entity->component<Skills>();
    if (skills_component != nullptr) {
        for (auto& [id, skill] : skills_component->skills()) {
            const auto label = Translator::translate(skill.label());
            i32 progress = skill.progress;
            if (ImGui::InputInt(label.c_str(), &progress)) {
                skills_component->set_progress(id, std::max(skill.progress, 0));
            }
        }
    }

    ImGui::PopItemWidth();
}

void SceneXray::entity_glyph(Entity* entity) {
    if (entity == nullptr) {
        return;
    }
    auto rc = entity->component<Render>();
    if (!rc) {
        return;
    }
    const auto& display_info = rc->display_info();
    const auto uvuv = Renderer::calculate_glyph_uv(display_info.glyph);
    const auto texture = Renderer::text_texture();
    const auto& color = display_info.color;
    const float glyph_width = 32.0f;
    ImGui::Image(
        reinterpret_cast<void*>(static_cast<intptr_t>(texture)),
        ImVec2(glyph_width, glyph_width / Renderer::glyph_aspect_ratio()),
        ImVec2(uvuv[0], uvuv[1]),
        ImVec2(uvuv[2], uvuv[3]),
        ImVec4(
            static_cast<float>(color.r) / 255.0f,
            static_cast<float>(color.b) / 255.0f,
            static_cast<float>(color.g) / 255.0f,
            1.0f
        )
    );
}
