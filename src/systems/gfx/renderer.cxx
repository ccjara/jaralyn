#include "renderer.hxx"

namespace {
    // main vertex shader source
    static const char* const vss = R"RAW(
#version 330 core
layout (location = 0) in vec3 pos;

void main() {
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
)RAW";

    // main fragment shader source
    static const char* const fss = R"RAW(
#version 330 core
out vec4 col;

void main() {
    col = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
)RAW";
}

renderer::renderer() :
    gl_context(nullptr),
    text(std::make_shared<text_renderer>()) {
}

renderer::~renderer() {
    if (gl_context != nullptr) {
        glDeleteBuffers(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);

        SDL_GL_DeleteContext(gl_context);
    }
}

void renderer::bind(const window* w) {
    window_ = w;

    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if (gl_context != nullptr) {
        SDL_GL_DeleteContext(gl_context);
    }

    gl_context = SDL_GL_CreateContext(w->get_handle());

    if (glewInit() != GLEW_OK) {
        LOG(ERROR) << "Could not initialize glew";
        throw;
    }

    // text->load();

    /////////////////////////////////////////////////////////////////

    main_shader_.load(vss, fss);

    // BUFFERS
    const float vertices[] = {
        0.5f, 0.0f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, // bottom left
       -0.5f, 0.0, 0.0f, // top left
    };

    const unsigned int indices[] = {
        0, 1, 3, 3, 2, 1
    };


    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    /////////////////////////////////////////////////////////////////

    SDL_GL_MakeCurrent(nullptr, nullptr);
}

void renderer::start_rendering() {
    SDL_GL_MakeCurrent(window_->get_handle(), gl_context);

    const auto size { window_->get_size() };

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, size.width, size.height);

    main_shader_.use();

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, static_cast<GLsizei> (size.width), static_cast<GLsizei> (size.height));
    glOrtho(0.0f, static_cast<GLdouble> (size.width), static_cast<GLdouble> (size.height), 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    */
}

void renderer::finish_rendering() {
    SDL_GL_SwapWindow(window_->get_handle());
    SDL_GL_MakeCurrent(nullptr, nullptr);
}
