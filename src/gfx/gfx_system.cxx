#include "gfx_system.hxx"

j_gfx_system::j_gfx_system(const j_window *w) {
    window_ = w;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    gl_context = SDL_GL_CreateContext(w->handle());

    if (gl_context == nullptr) {
        LOG(ERROR) << "Could not initialize opengl";
        throw;
    }

    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        LOG(ERROR) << "Could not initialize glew";
        throw;
    }

    renderer_->bind(window_);

    const auto s { window_->size() };

    display_.resize({ s.width / 8, s.height / 14 });
}

j_gfx_system::~j_gfx_system() {
    if (gl_context != nullptr) {
        SDL_GL_DeleteContext(gl_context);
    }
}

void j_gfx_system::prepare() {
    fps_.pre_render();

    display_.reset();
}

void j_gfx_system::present() {
    j_rect_options frame_opt;

    const auto dim { display_.dimensions() };
    frame_opt.color = { 128, 128, 128, 255 };
    frame_opt.span = j_rect<uint32_t>(0, dim.width - 1, dim.height - 1, 0);

    std::stringstream ss;
    ss.precision(3);
    ss << "MSPF: " << fps_.mspf() << " (" << fps_.get_fps() << " FPS)";


    display_.rectangle(frame_opt);
    display_.text(ss.str(), { j_rect<uint32_t>(0, 0, 0, 2) });

    renderer_->render(display_);
}

j_renderer& j_gfx_system::renderer() noexcept {
    return *renderer_;
}

j_display& j_gfx_system::display() noexcept {
    return display_;
}
