#ifndef JARALYN_SKILLS_COMPONENT_HXX
#define JARALYN_SKILLS_COMPONENT_HXX

#include "component/generic_component.hxx"

enum class SkillId {
    Swim,
};

const std::unordered_map<SkillId, const char*> skill_labels = {
    { SkillId::Swim, "SKILL_SWIMMING" }
};

struct Skill {
    SkillId id;
    i32 level = 0;
    i32 progress = 0;

    const char* label() const {
        const auto& iter = skill_labels.find(id);
        if (iter == skill_labels.end()) {
            return "SKILL_UNKNOWN";
        }
        return iter->second;
    }
};


class Skills : public GenericComponent<Skills, ComponentType::Skills> {
    using SkillContainer = std::unordered_map<SkillId, Skill>;
public:
    /**
     * @brief Sets the progress of a skill.
     */
    void set_progress(SkillId id, i32 amount);

    /**
     * @brief Adds the given amount to a skill's progress.
     *
     * If negative the amount will be deducted instead.
     */
    void increase_progress(SkillId id, i32 amount);

    const SkillContainer& skills() const;
protected:
    SkillContainer skills_;
};

#endif
