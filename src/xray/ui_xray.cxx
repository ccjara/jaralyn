#include "ui_xray.hxx"

UiXray::UiXray(EventManager& dispatcher, Ui& ui) :
    ui_ { ui },
    events_ { dispatcher } {
}

/**
 * @brief Recursively renders the given node and its anchored nodes
 *
 * Returns false if anchoring has altered and recursive rendering must
 * be cancelled due to reference invalidation.
 */
bool render_anchor(UiNode* node) {
    if (!node) {
        return true;
    }
    const auto& anchored_by { node->anchored_by() };
    const bool has_anchors { !anchored_by.empty() };
    const auto imgui_tree_node_flags {
        has_anchors ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_Leaf
    };
    const auto id { node->id().data() };

    if (ImGui::TreeNodeEx(id, imgui_tree_node_flags, id)) {
        if (ImGui::BeginDragDropTarget()) {
            if (auto payload = ImGui::AcceptDragDropPayload("_TREENODE")) {
                UiNode* accepted_node { nullptr };
                std::memcpy(&accepted_node, payload->Data, sizeof(UiNode*));
                if (accepted_node->can_anchor_to(node)) {
                    accepted_node->anchor_to(*node);
                    ImGui::EndDragDropTarget();
                    ImGui::TreePop();
                    return false;
                }
            }
            ImGui::EndDragDropTarget();
        }
        if (!node->is_root() && ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("_TREENODE", &node, sizeof(UiNode*));
            ImGui::Text("Drag to a new anchor node");
            ImGui::EndDragDropSource();
        }
        for (const auto& n : anchored_by) {
            if (!render_anchor(n)) {
                ImGui::TreePop();
                return false;
            }
        }
        ImGui::TreePop();
    }
    return true;
}

void render_hierarchy(UiNode* node) {
    if (!node) {
        return;
    }
    if (ImGui::TreeNode(node->id().data())) {
        for (const auto& n : node->children()) {
            render_hierarchy(n);
        }
        ImGui::TreePop();
    }
}

void UiXray::update() {
    ImGui::Begin("UI");

    if (ImGui::CollapsingHeader("Hierarchy")) {
        render_hierarchy(ui_.ui_tree().root());
    }

    if (ImGui::CollapsingHeader("Anchors")) {
        render_anchor(ui_.ui_tree().root());
    }

    ImGui::End();
}
