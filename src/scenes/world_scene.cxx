#include "world_scene.hxx"

j_world_scene::j_world_scene() {
    auto player { registry_.create() };

    registry_.assign<jc_position>(player, 10, 10, 0);
    registry_.assign<jc_renderable>(player, static_cast<unsigned char>('@'));
}

void j_world_scene::update(const j_input_state& input) {
    j_vec2<int32_t> vel;

    if (input.keyboard().is_pressed(SDL_KeyCode::SDLK_w)) {
        vel.y -= 1;
    }
    if (input.keyboard().is_pressed(SDL_KeyCode::SDLK_s)) {
        vel.y += 1;
    }
    if (input.keyboard().is_pressed(SDL_KeyCode::SDLK_a)) {
        vel.x -= 1;
    }
    if (input.keyboard().is_pressed(SDL_KeyCode::SDLK_d)) {
        vel.x += 1;
    }

    if (!vel.x && !vel.y) {
        return;
    }

    registry_.view<jc_position>().each([&vel = std::as_const(vel)](auto& position) {
        position.x += vel.x;
        position.y += vel.y;
    });
}

void j_world_scene::render(j_display& display) {
    registry_.view<jc_renderable, jc_position>().each([&display](auto& renderable, auto& position) {
        j_position<uint32_t> pos {
            static_cast<uint32_t> (position.x),
            static_cast<uint32_t> (position.y)
        };

        if (position.x < 0 || position.y < 0 || !display.in_bounds(pos)) {
            return;
        }

        j_display_cell cell;

        cell.color = renderable.color;
        cell.glyph = renderable.glyph;

        display.put(std::move(cell), std::move(pos));
    });
}
