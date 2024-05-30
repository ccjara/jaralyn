#include "ai/ai_script.hxx"

void AiScript::clear() {
    // TODO: script invocation
    mod_state(AiNodeState::Ready);
}

AiNodeState AiScript::visit(AiContext& context) {
    return mod_state(AiNodeState::Succeeded);
}
