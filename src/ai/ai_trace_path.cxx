#include "ai_trace_path.hxx"

void AiTracePath::clear() {
    search_ = AStarSearch();
}

AiNodeState AiTracePath::visit(AiContext& context) {
    // TODO: do the search
    return mod_state(AiNodeState::Succeeded);
}

const AStarSearch& AiTracePath::search() const {
    return search_;
}
