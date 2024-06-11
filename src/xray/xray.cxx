#include "xray/xray.hxx"

void Xray::init(Events* events) {
    assert(events);

    mouse_down_sub_ = events->engine->on<MouseDownEvent>(&Xray::on_mouse_down, 10000);
    mouse_up_sub_ = events->engine->on<MouseUpEvent>(&Xray::on_mouse_up, 10000);
    key_down_sub_ = events->engine->on<KeyDownEvent>(&Xray::on_key_down, 10000);
    key_up_sub_ = events->engine->on<KeyUpEvent>(&Xray::on_key_up, 10000);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Xray::shutdown() {
    xrays_.clear();

    mouse_down_sub_.unsubscribe();
    mouse_up_sub_.unsubscribe();
    key_down_sub_.unsubscribe();
    key_up_sub_.unsubscribe();
}

EventResult Xray::on_mouse_down(MouseDownEvent &e) {
    return ImGui::GetIO().WantCaptureMouse
        ? EventResult::Halt
        : EventResult::Continue;
}

EventResult Xray::on_mouse_up(MouseUpEvent &e) {
    return ImGui::GetIO().WantCaptureMouse
        ? EventResult::Halt
        : EventResult::Continue;
}

EventResult Xray::on_key_down(KeyDownEvent &e) {
    if (ImGui::GetIO().WantCaptureKeyboard) {
        return EventResult::Halt;
    }
    if (e.key == Key::F1) {
        show_xray_ = !show_xray_;
        return EventResult::Halt;
    }
    return EventResult::Continue;
}

EventResult Xray::on_key_up(KeyUpEvent &e) {
    return ImGui::GetIO().WantCaptureKeyboard
        ? EventResult::Halt
        : EventResult::Continue;
}

void Xray::draw() {
    if (!show_xray_) {
        ImGui::Render();
        return;
    }

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 { 0.0f, 0.0f });

    constexpr auto window_flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("ImGuiMainWindow", nullptr, window_flags);
    ImGui::PopStyleVar(1);

    const auto dock_id { ImGui::GetID("XrayDock") };
    ImGui::DockSpace(dock_id, ImVec2{}, ImGuiDockNodeFlags_PassthruCentralNode);

    for (const auto& xray : xrays_) {
        xray->update();
    }

    ImGui::End();
    ImGui::Render();
}
