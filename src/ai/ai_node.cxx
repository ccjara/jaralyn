#include "ai/ai_node.hxx"

AiNodeState AiNode::mod_state(AiNodeState s) {
    state_ = s;
    return s;
}

AiNodeState AiNode::state() const {
    return state_;
}

bool AiNode::failed() const {
    return state_ == AiNodeState::Failed;
}
