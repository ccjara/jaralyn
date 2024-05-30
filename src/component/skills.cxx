#include "component/skills.hxx"

const Skills::SkillContainer& Skills::skills() const {
    return skills_;
}

void Skills::increase_progress(SkillId id, i32 amount) {
    skills_[id].progress += amount;
}

void Skills::set_progress(SkillId id, i32 amount) {
    skills_[id].progress = amount;
}
