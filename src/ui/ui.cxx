#include "ui.hxx"

void Ui::init() {
    ui_tree_.create_root_node();

    Events::on<DisplayResizedEvent>(&Ui::on_display_resized);
    Events::on<ScriptResetEvent>(&Ui::on_script_reset);
    Events::on<PostWorldRenderEvent>(&Ui::on_post_world_render);
}

void Ui::shutdown() {
    ui_tree_.clear();
}

void Ui::update() {
    for (auto& node : ui_tree_.nodes()) {
        const Id handler = node->handler_id();
        if (handler != null_id) {
            Scripting::invoke_callback(handler);
        }
    }
}

bool Ui::on_display_resized(DisplayResizedEvent& e) {
    if (auto root = ui_tree_.root()) {
        root->resize(e.size);
    }
    return false;
}

bool Ui::on_script_reset(ScriptResetEvent& e) {
    ui_tree_.reset();
    return false;
}

bool Ui::on_post_world_render(PostWorldRenderEvent& e) {
    draw_node(ui_tree_.root());
    return false;
}

UiTree& Ui::tree() {
    return ui_tree_;
}

void Ui::draw_node(UiNode* node) {
    assert(node);
    if (!node->visible()) {
        return;
    }
    auto& display = Renderer::display();

    if (node->type() == UiNodeType::window) {
        auto n = static_cast<UiWindow*>(node);
        const auto pos = n->absolute_position();
        // border
        display.rectangle({
            Rect<i32> { pos, n->size() },
            Color::mono(128),
            Color::black(),
        });
        // title
        display.text(n->title(), pos);
    } else if (node->type() == UiNodeType::text) {
        auto n = static_cast<UiText*>(node);
        display.text(n->text(), n->absolute_position());
    }
    for (UiNode* child_node : node->children()) {
        draw_node(child_node);
    }
}
