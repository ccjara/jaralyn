#include "ai/ai_sequence.hxx"

AiSequence& AiSequence::operator=(AiSequence other) {
    swap(*this, other);
    return *this;
}

AiSequence::AiSequence(const AiSequence& other) {
    nodes_ = other.nodes_;
    iter = nodes_.begin();
}

AiSequence::AiSequence(AiSequence&& other) : AiSequence() {
    swap(*this, other);
}

AiNodeState AiSequence::visit(AiContext& context) {
    mod_state(AiNodeState::Running);
    iter = nodes_.begin();

    while (iter != nodes_.end()) {
        assert(iter->ptr);
        const auto child_state = iter->ptr->visit(context);

        if (child_state == AiNodeState::Failed) {
            return mod_state(AiNodeState::Failed);
        }
        // continue iteration, trying the next node available
        ++iter;
    }
    return mod_state(AiNodeState::Succeeded);
}

void AiSequence::clear() {
    for (auto& n : nodes_) {
        n.ptr->clear();
    }
    iter = nodes_.begin();
    mod_state(AiNodeState::Ready);
}

void AiSequence::post_create_node() {
    // inefficient, but shouldn't get us into trouble unless we construct
    // massive behavior trees
    // enter poor man's priority queue
    std::sort(
        nodes_.begin(),
        nodes_.end(),
        [](const auto& a, const auto& b) -> bool {
            return a.priority > b.priority;
        }
    );
    // iterator must be invalidated as elements might get reordered
    iter = nodes_.begin();
}


// -- PrioritizedNode

// default
AiSequence::PrioritizedNode::PrioritizedNode(std::unique_ptr<AiNode>&& ptr, i32 priority) :
    ptr(std::move(ptr)),
    priority(priority) {
    assert(this->ptr);
}

// copy
AiSequence::PrioritizedNode::PrioritizedNode(const AiSequence::PrioritizedNode& other) {
    ptr = other.ptr->clone();
    priority = other.priority;
}

// move
AiSequence::PrioritizedNode::PrioritizedNode(AiSequence::PrioritizedNode&& other) : AiSequence::PrioritizedNode() {
    swap(*this, other);
}

// cas
AiSequence::PrioritizedNode& AiSequence::PrioritizedNode::operator=(AiSequence::PrioritizedNode other) {
    swap(*this, other);
    return *this;
}
