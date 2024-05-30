#include "xray/script_xray.hxx"

ScriptXray::ScriptXray(EventManager* events) {
    assert(events);
    events->on<ScriptLoadedEvent>(this, &ScriptXray::on_script_loaded);
    events->on<ScriptResetEvent>(this, &ScriptXray::on_script_reset);
}

bool ScriptXray::on_script_loaded(ScriptLoadedEvent& e) {
    return false;
}

bool ScriptXray::on_script_reset(ScriptResetEvent& e) {
    selected_script_id_ = std::nullopt;
    return false;
}

void ScriptXray::update() {
    update_script_globals_table();

    ImGui::Begin("Scripts");
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_GREEN);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_GREEN_HOVER);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_GREEN_ACTIVE);
    if (ImGui::Button("[F5] Reload")) {
        Scripting::load_from_path(Scripting::default_script_path);
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    current_script_ = selected_script_id_ ? Scripting::get_by_id(*selected_script_id_) : nullptr;

    const auto combo_label {
         current_script_ ? current_script_->name().c_str() : "Choose script"
    };

    if (ImGui::BeginCombo("Script", combo_label)) {
        for (const auto& [name, script] : Scripting::scripts()) {
            const bool is_selected {
                selected_script_id_ && *selected_script_id_ == script->id
            };
            if (ImGui::Selectable(script->name().c_str(), is_selected)) {
                selected_script_id_ = script->id;
            }
        }
        ImGui::EndCombo();
    }

    render_current_script(current_script_);
    ImGui::End();
}

void ScriptXray::update_script_globals_table() {
    if (!globals_.visible || !current_script_) {
        return;
    }
    const int globals_refresh_ticks = 60;
    const int ticks = globals_.ticks++;

    if (globals_.ticks == globals_refresh_ticks) {
        globals_.ticks = 0;
    }

    if (ticks != 0) {
        return;
    }
    globals_.rows.clear();

    lua_getglobal(*current_script_, "_G");
    lua_pushnil(*current_script_);

    while (lua_next(*current_script_, -2) != 0) {
        // key is at index -2 and value is at index -1
        const char *key = lua_tostring(*current_script_, -2);
        const int type = lua_type(*current_script_, -1);

        std::string value;

        switch (type) {
            case LUA_TBOOLEAN:
                value = lua_toboolean(*current_script_, -1) == 0 ? "false" : "true";
                break;
            case LUA_TNUMBER:
                value = fmt::to_string(lua_tonumber(*current_script_, -1));
                break;
            case LUA_TSTRING:
                value = lua_tostring(*current_script_, -1);
                break;
            default:
                value = "?";
        }

        globals_.rows.emplace_back(
            std::make_tuple(
                key,
                lua_typename(*current_script_, type), 
                value.c_str()
            )
        );
        // remove value, keep key for the next iteration
        lua_pop(*current_script_, 1);
    }
    lua_pop(*current_script_, 1);

    std::sort(
        globals_.rows.begin(),
        globals_.rows.end(),
        [](const auto& a, const auto& b) {
            std::string str_a = std::get<0>(a);
            std::string str_b = std::get<0>(b);

            for (auto& c : str_a) {
                c = std::tolower(c);
            }
            for (auto& c : str_b) {
                c = std::tolower(c);
            }

            return str_a < str_b;
        }
    );
}

void ScriptXray::render_current_script(Script *current) {
    if (!current) {
        return;
    }
    if (globals_.visible = ImGui::TreeNode("Globals")) {
        if (ImGui::BeginTable("log_table", 3)) {
            ImGui::TableSetupScrollFreeze(0, 1); // keep header in view
            ImGui::TableSetupColumn("Key");
            ImGui::TableSetupColumn("Type");
            ImGui::TableSetupColumn("Value");
            ImGui::TableHeadersRow();

            for (const auto& [name, type, value] : globals_.rows) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(name.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(type.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(value.c_str());
            }

            ImGui::EndTable();
            ImGui::TreePop();
        }
    }
}
