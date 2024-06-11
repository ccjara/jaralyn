#include "ui/ui.hxx"
#include "gfx/display.hxx"

void Ui::init(Events* events, Display* display) {
    assert(events);
    assert(display);

    events_ = events;
    display_ = display;

    ui_tree_.create_root_node();

    display_resized_sub_ = events_->engine->on<DisplayResizedEvent>(&Ui::on_display_resized);
    script_reset_sub_ = events_->engine->on<ScriptResetEvent>(&Ui::on_script_reset);
}

void Ui::shutdown() {
    display_ = nullptr;
    ui_tree_.clear();

    display_resized_sub_.unsubscribe();
    script_reset_sub_.unsubscribe();
}

void Ui::update() {
    update_node(ui_tree_.root());
}

void Ui::draw() {
    draw_node(ui_tree_.root());
}

void Ui::update_node(UiNode* node) {
    if (node == nullptr) {
        return;
    }
    node->call_handler();

    for (UiNode* node : node->children()) {
        update_node(node);
    }
}

EventResult Ui::on_display_resized(DisplayResizedEvent& e) {
    if (auto root = ui_tree_.root()) {
        root->resize(e.size);
    }
    return EventResult::Continue;
}

EventResult Ui::on_script_reset(ScriptResetEvent& e) {
    ui_tree_.reset();
    return EventResult::Continue;
}

UiTree& Ui::tree() {
    return ui_tree_;
}

void Ui::draw_node(UiNode* node) {
    assert(node);
    if (!node->visible()) {
        return;
    }

    if (node->type() == UiNodeType::window) {
        auto n = static_cast<UiWindow*>(node);
        const auto pos = n->absolute_position();
        // border
        display_->rectangle({
            Rect<i32> { pos, n->size() },
            Color::mono(128),
            Color::black(),
        });
        // title
        display_->text(n->title(), pos);
    } else if (node->type() == UiNodeType::text) {
        auto n = static_cast<UiText*>(node);
        display_->text(n->text(), n->absolute_position());
    }
    for (UiNode* child_node : node->children()) {
        draw_node(child_node);
    }
}
