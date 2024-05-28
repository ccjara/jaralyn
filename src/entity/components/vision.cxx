#include "vision.hxx"
#include "../../scene/scene.hxx"

void Vision::update(u64 dt) {
}

void Vision::set_vision_radius(i32 radius) {
    fov_.set_vision_radius(std::max(radius, 1));
}

i32 Vision::vision_radius() const {
    return fov_.vision_radius();
}

const Fov& Vision::fov() const {
    return fov_;
}

void Vision::on_after_actions() {
    update_fov();
}

bool Vision::applied() const {
    return apply_;
}

void Vision::set_apply(bool apply_vision) {
    apply_ = apply_vision;
}

void Vision::on_player_attached() {
    apply_ = true;
    update_fov();
}

void Vision::on_player_detached() {
    apply_ = false;
}

void Vision::update_fov() {
    if (entity_ == nullptr) {
        return;
    }
    fov_.update(*entity_, Scene::tiles());

    if (apply_) {
        Scene::apply_fov(entity_->position, fov_);
    }
}
