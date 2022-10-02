#include "script_xray.hxx"

ScriptXray::ScriptXray() {
    Events::on<ScriptLoadedEvent>(this, &ScriptXray::on_script_loaded);
    Events::on<ScriptResetEvent>(this, &ScriptXray::on_script_reset);
}

bool ScriptXray::on_script_loaded(ScriptLoadedEvent& e) {
    return false;
}

bool ScriptXray::on_script_reset(ScriptResetEvent& e) {
    selected_script_id_ = std::nullopt;
    return false;
}

void ScriptXray::update() {
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

    Script* current_script {
        selected_script_id_ ? Scripting::get_by_id(*selected_script_id_) : nullptr
    };
    const auto combo_label {
         current_script ? current_script->name().c_str() : "Choose script"
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

    render_current_script(current_script);
    ImGui::End();
}

void ScriptXray::render_current_script(Script *current) {
    if (!current) {
        return;
    }
}
