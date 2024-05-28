#include "xray.hxx"
#include "../events/event_manager.hxx"

void Xray::init(EventManager* events, SDL_GLContext context) {
    assert(events);
    imgui_context_ = ImGui::CreateContext();
    if (!imgui_context_) {
        Log::error("Could not create imgui context");
        return;
    }
    events->on<PostRenderEvent>(&Xray::on_post_render);
    events->on<MouseDownEvent>(&Xray::on_mouse_down, 10000);
    events->on<MouseUpEvent>(&Xray::on_mouse_up, 10000);
    events->on<KeyDownEvent>(&Xray::on_key_down, 10000);
    events->on<KeyUpEvent>(&Xray::on_key_up, 10000);

    ImGui_ImplSDL2_InitForOpenGL(Window::handle(), context);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Xray::shutdown() {
    xrays_.clear();
    if (imgui_context_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext(imgui_context_);
        imgui_context_ = nullptr;
    }
}

bool Xray::on_mouse_down(MouseDownEvent &e) {
    return ImGui::GetIO().WantCaptureMouse;
}

bool Xray::on_mouse_up(MouseUpEvent &e) {
    return ImGui::GetIO().WantCaptureMouse;
}

bool Xray::on_key_down(KeyDownEvent &e) {
    if (ImGui::GetIO().WantCaptureKeyboard) {
        return true;
    }
    if (e.key == Key::F1) {
        show_xray_ = !show_xray_;
        return true;
    }
    return false;
}

bool Xray::on_key_up(KeyUpEvent &e) {
    return ImGui::GetIO().WantCaptureKeyboard;
}

bool Xray::on_post_render(PostRenderEvent&) {
    if (!show_xray_) {
        return false;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(Window::handle());
    ImGui::NewFrame();

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
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return false;
}
