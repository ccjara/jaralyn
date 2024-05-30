#include "ai/ai_condition.hxx"

AiCondition::AiCondition(ConditionFn condition) : condition_(std::move(condition)) {
}

AiNodeState AiCondition::visit(AiContext& context) {
    if (condition_(context)) {
        return mod_state(AiNodeState::Succeeded);
    }
    return mod_state(AiNodeState::Failed);
}

void AiCondition::clear() {
    mod_state(AiNodeState::Ready);
}
