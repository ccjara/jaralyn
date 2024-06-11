#include "gfx/renderer.hxx"

void Renderer::shutdown() {
    if (vao) {
        glDeleteBuffers(1, &vao);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }

    resize_sub_.unsubscribe();
    config_updated_sub_.unsubscribe();
}

void Renderer::init(Events* events) {
    assert(events);

    events_ = events;
    resize_sub_ = events_->engine->on<ResizeEvent>(&Renderer::on_resize);
    config_updated_sub_ = events_->engine->on<ConfigUpdatedEvent>(&Renderer::on_config_updated);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glyph
    glVertexAttribIPointer(0, 1, GL_INT, sizeof(Display::cell_type), nullptr);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Display::cell_type), reinterpret_cast<void*> (sizeof(Display::cell_type::glyph)));
    glEnableVertexAttribArray(1);

    text_shader_ = std::make_unique<TextShader>();

    configure(Config());
}

void Renderer::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, view_port_.x, view_port_.y);

    text_shader_->use();

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    for (auto& layer : layers_) {
        const auto byte_size { layer.byte_size() };

        if (last_size_ == layer.byte_size()) {
            glBufferSubData(GL_ARRAY_BUFFER, 0, byte_size, layer.data());
        } else {
            glBufferData(GL_ARRAY_BUFFER, byte_size, layer.data(), GL_DYNAMIC_DRAW);

            last_size_ = byte_size;
        }
        glDrawArrays(GL_POINTS, 0, layer.cell_count());
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::set_viewport(Vec2<u32> size) {
    view_port_ = size;
    text_shader_->use_resolution(view_port_ / scaling_);
}

void Renderer::set_font(Texture* tex) {
    text_shader_->use_texture(tex);
}

void Renderer::set_glyph_size(Vec2<u32> glyph_size) {
    text_shader_->use_glyph_size(glyph_size);
}

void Renderer::set_scaling(u32 scaling) {
    scaling_ = scaling;
    text_shader_->use_resolution(view_port_ / scaling);
}

EventResult Renderer::on_resize(ResizeEvent& e) {
    set_viewport(e.size);
    adjust_display();
    return EventResult::Continue;
}

void Renderer::configure(const Config& cfg) {
    cfg_ = cfg;

    const auto path_str = cfg_.font_texture_path.string();
    if (!fs::exists(cfg_.font_texture_path)) {
        Log::error("Could not read text font at path {}", path_str);
        std::abort();
    }
    text_texture_.load(path_str);

    set_font(&text_texture_);
    set_glyph_size(cfg_.glyph_size);
    set_scaling(cfg_.scaling);
    adjust_display();
}

EventResult Renderer::on_config_updated(ConfigUpdatedEvent& e) {
    configure(e.next);
    return EventResult::Continue;
}

void Renderer::adjust_display() {
    // calculate resolution
    const auto scaled_size { view_port_ / cfg_.scaling };
    // calculate how many cells will fit on the screen given that resolution
    const Vec2<u32> display_size {
        scaled_size.x / cfg_.glyph_size.x,
        scaled_size.y / cfg_.glyph_size.y
    };
    // resize and notify
    for (auto& layer : layers_) {
        layer.resize(display_size);
    }

    events_->engine->trigger<DisplayResizedEvent>(display_size);
    Log::debug("Display resized to {}x{} cells", display_size.x, display_size.y);
}

std::array<float, 4> Renderer::calculate_glyph_uv(u32 glyph) {
    const Vec2<float> glyph_size = {
        static_cast<float>(text_shader_->glyph_size().x),
        static_cast<float>(text_shader_->glyph_size().y)
    };
    const Vec2<float> tex_size = {
        static_cast<float>(text_shader_->texture_size().x),
        static_cast<float>(text_shader_->texture_size().y)
    };
    const int chars_per_row_in_texture = static_cast<i32>(tex_size.x / glyph_size.x);

    const Vec2<float> tex_origin {
        static_cast<float>(glyph % chars_per_row_in_texture) * glyph_size.x,
        static_cast<float>(std::floor(glyph / chars_per_row_in_texture)) * glyph_size.y
    };

    const float u1 = 1.0f / tex_size.x * tex_origin.x;
    const float u2 = 1.0f / tex_size.x * (tex_origin.x + glyph_size.x);
    const float v1 = 1.0f / tex_size.y * tex_origin.y;
    const float v2 = 1.0f / tex_size.y * (tex_origin.y + glyph_size.y);

    return std::array<float, 4> { u1, v1, u2, v2 };
}

GLuint Renderer::text_texture() {
    return text_texture_.id();
}

float Renderer::glyph_aspect_ratio() {
    return static_cast<float>(
        text_shader_->glyph_size().x
    ) / text_shader_->glyph_size().y;
}

Display& Renderer::display() {
    return layers_[0];
}

Display& Renderer::ui_layer() {
    return layers_[1];
}
