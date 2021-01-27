#include "hud_system.hxx"

void j_hud_system::on_load() {
    define_task<j_gathering_completed_event, &j_hud_system::task_journal_item_pickup>();
    define_task<j_inventory_view_event, &j_hud_system::task_show_inventory_ui>();
}

void j_hud_system::update(uint32_t delta_time) {
    queue_.update();
}

const std::vector<std::string>& j_hud_system::journal_entries() const {
    return journal_.entries();
}

void j_hud_system::task_journal_item_pickup(const j_gathering_completed_event& e) {
    std::string log_str { "Picked up " };

    if (game->entities()->has<jc_object_descriptor>(e.item)) {
        log_str += "$c00ff00";
        log_str += game->entities()->get<jc_object_descriptor>(e.item).label;
        log_str += "$!";
    }
    else {
        log_str += " something";
    }

    journal_.emplace_back(std::move(log_str));
}

void j_hud_system::task_show_inventory_ui(const j_inventory_view_event& e) {
    if (!inventory_window_) {
        // TODO: create j_display_resized event or some sort to update the root's size
        ui_.root()->resize({ 120, 38 });

        inventory_window_ = ui_.create_window(ui_.root(), "inventory_window");
        assert(inventory_window_);

        inventory_window_->set_title("Inventory");
        inventory_window_->set_anchor_origin(j_ui_anchor_origin::center);
        inventory_window_->resize({ 20, 10 });
        inventory_window_->move({ 0, 0 });
    }
}

const j_ui_tree& j_hud_system::ui_tree() const {
    return ui_;
}