#ifndef JARALYN_BEHAVIOR_HXX
#define JARALYN_BEHAVIOR_HXX

#include "generic_component.hxx"
#include "../../ai/ai_node.hxx"

class Behavior : public GenericComponent<Behavior, ComponentType::Behavior> {
public:
    Behavior() = default;
    Behavior(const Behavior& other);
    Behavior(Behavior&& other);

    Behavior& operator=(Behavior other);

    friend void swap(Behavior& lhs, Behavior& rhs) {
        using std::swap;
        swap(lhs.root_, rhs.root_);
        swap(lhs.context_, rhs.context_);
    }

    /**
     * @brief Runs the next behavior cycle
     */
    void update(u64 dt) override;

    /**
     * @brief Takes ownership of the given AiNode unique_ptr and overwrites the current root
     */
    void set_root(std::unique_ptr<AiNode>&& root);

    /**
     * @brief Assigns the entity id and tracks it in the AiContext
     */
    void set_entity_id(Id id) override;

    /**
     * @brief Sets the behavior tree root.
     *
     * If a root node already exists, it will be overwritten.
     * Returns the node for further modification.
     */
    template<typename Node, typename... NodeArgs>
    AiNode& create_root(NodeArgs&&... args) {
        root_.reset(new Node(std::forward<NodeArgs>(args)...));
        if (root_) {
            root_.release();
        }
        return *root_.get();
    }

    [[nodiscard]] AiNode* root() const;
private:
    AiContext context_;
    std::unique_ptr<AiNode> root_;
};

#endif