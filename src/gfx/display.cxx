#include "display.hxx"

const j_display_cell j_display_cell::null = j_display_cell();

void j_display::text(const std::string& t, const j_text_options& options) {
    j_position<uint32_t> position {
        options.boundary.left,
        options.boundary.top
    };
    j_position<uint32_t> limit {
        options.boundary.right,
        options.boundary.bottom
    };
    if (!in_bounds(position) || limit.x < position.x || limit.y < position.y) {
        return;
    }
    if (!limit.x || limit.x > dimensions_.width) {
        limit.x = dimensions_.width;
    }
    if (!limit.y || limit.y > dimensions_.height) {
        limit.y = dimensions_.height;
    }
    const size_t text_length { t.length() };

    const auto needs_break = [&](size_t i) -> bool {
        auto x { position.x };

        while (x <= limit.x && i < text_length) {
            switch (t[i]) {
            case ' ':
            case '!':
            case '?':
            case '.':
            case ':':
            case ',':
            case ';':
                return false;
            }
            x++;
            i++;
        }
        return true;
    };

    for (size_t i = 0; i < text_length; i++) {
        if (position.y > limit.y) {
            break;
        }
        bool is_last_char_of_line { position.x == limit.x };
        const char c { t[i] };

        if (options.text_break == j_text_break::break_word) {
            if (c == ' ' && needs_break(i + 1)) {
                is_last_char_of_line = true;
            } else {
                cells_.at(to_index(position)).glyph = c;
            }
        } else {
            cells_.at(to_index(position)).glyph = c;
        }

        if (is_last_char_of_line) {
            position.x = options.boundary.left;
            position.y++;

            if (i + 1 < text_length && t[i + 1] == ' ') {
                // skip immediate space at new line
                i++;
                continue;
            }
        } else {
            position.x++;
        }
    }
}
