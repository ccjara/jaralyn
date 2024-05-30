#include "ai/ai_selector.hxx"

AiSelector& AiSelector::operator=(AiSelector other) {
    swap(*this, other);
    return *this;
}

AiSelector::AiSelector(const AiSelector& other) {
    nodes_ = other.nodes_;
    iter = nodes_.begin();
}

AiSelector::AiSelector(AiSelector&& other) : AiSelector() {
    swap(*this, other);
}

AiNodeState AiSelector::visit(AiContext& context) {
    mod_state(AiNodeState::Running);
    iter = nodes_.begin();

    while (iter != nodes_.end()) {
        assert(iter->ptr);
        const auto child_state = iter->ptr->visit(context);

        if (child_state == AiNodeState::Succeeded) {
            return mod_state(AiNodeState::Succeeded);
        }
        // continue iteration, trying the next node available
        ++iter;
    }
    return mod_state(AiNodeState::Failed);
}

void AiSelector::clear() {
    for (auto& n : nodes_) {
        n.ptr->clear();
    }
    iter = nodes_.begin();
    mod_state(AiNodeState::Ready);
}

void AiSelector::post_create_node() {
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
AiSelector::PrioritizedNode::PrioritizedNode(std::unique_ptr<AiNode>&& ptr, i32 priority) :
    ptr(std::move(ptr)),
    priority(priority) {
    assert(this->ptr);
}

// copy
AiSelector::PrioritizedNode::PrioritizedNode(const AiSelector::PrioritizedNode& other) {
    ptr = other.ptr->clone();
    priority = other.priority;
}

// move
AiSelector::PrioritizedNode::PrioritizedNode(AiSelector::PrioritizedNode&& other) : AiSelector::PrioritizedNode() {
    swap(*this, other);
}

// cas
AiSelector::PrioritizedNode& AiSelector::PrioritizedNode::operator=(AiSelector::PrioritizedNode other) {
    swap(*this, other);
    return *this;
}
