#include "platform/platform.hxx"
#include "platform/platform_event.hxx"
#include "input/input.hxx"
#include "input/input_event.hxx"

Platform::Platform(EventManager* events, Input *input) : events_(events), input_(input) {
    assert(events_);
    assert(input_);
}

void Platform::initialize() {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Log::error("Failed to initialize SDL: %s", SDL_GetError());
        std::abort();
    }
    sdl_initialized_ = true;

    sdl_window_ = SDL_CreateWindow(
        "Jaralyn",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1600,
        900,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!sdl_window_) {
        Log::error("Failed to create SDL window: %s", SDL_GetError());
        std::abort();
    }

    gl_context_ = SDL_GL_CreateContext(sdl_window_);
    if (!gl_context_) {
        Log::error("Failed to create GL context: %s", SDL_GetError());
        std::abort();
    }
    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        Log::error("Could not initialize glew");
        std::abort();
    }
    imgui_context_ = ImGui::CreateContext();
    if (!imgui_context_) {
        Log::error("Could not create imgui context");
    }
    ImGui_ImplSDL2_InitForOpenGL(sdl_window_, gl_context_);
    ImGui_ImplOpenGL3_Init();
}

bool Platform::prepare() {
    if (!process_events()) {
        return false;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(sdl_window_);
    ImGui::NewFrame();
    return true;
}

void Platform::present() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(sdl_window_);
}

void Platform::shutdown() {
    if (imgui_context_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext(imgui_context_);
        imgui_context_ = nullptr;
    }

    if (gl_context_) {
        SDL_GL_DeleteContext(gl_context_);
        gl_context_ = nullptr;
    }

    if (sdl_window_) {
        SDL_DestroyWindow(sdl_window_);
        sdl_window_ = nullptr;
    }

    if (sdl_initialized_) {
        SDL_Quit();
        sdl_initialized_ = false;
    }    
}

Platform::~Platform() {
    shutdown();
}

Vec2<u32> Platform::window_size() const {
    i32 w, h;
    SDL_GetWindowSize(sdl_window_, &w, &h);
    return { static_cast<u32>(w), static_cast<u32>(h) };
}

bool Platform::process_events() {
    SDL_PumpEvents();

    SDL_Event e;
    while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_EventType::SDL_FIRSTEVENT, SDL_EventType::SDL_MOUSEWHEEL) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&e);

        switch (e.type) {
            case SDL_EventType::SDL_QUIT:
                return false;
            case SDL_EventType::SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    events_->trigger<ResizeEvent>(Vec2<i32> {
                        static_cast<i32> (e.window.data1),
                        static_cast<i32> (e.window.data2)
                    });
                }
                break;
            case SDL_EventType::SDL_KEYDOWN:
                input_->set_key_pressed(static_cast<Key>(e.key.keysym.sym), true);
                break;
            case SDL_EventType::SDL_KEYUP:
                input_->set_key_pressed(static_cast<Key>(e.key.keysym.sym), false);
                break;
            case SDL_EventType::SDL_MOUSEBUTTONDOWN:
                input_->set_mouse_button_pressed(static_cast<MouseButton>(e.button.button), true);
                break;
            case SDL_EventType::SDL_MOUSEBUTTONUP:
                input_->set_mouse_button_pressed(static_cast<MouseButton>(e.button.button), false);
                break;
            case SDL_EventType::SDL_MOUSEMOTION:
                input_->set_mouse_position({ e.motion.x, e.motion.y });
                break;
        }
    }
    return true;
}