#ifndef JARALYN_BEHAVIOR_HXX
#define JARALYN_BEHAVIOR_HXX

#include "component/generic_component.hxx"
#include "ai/ai_context.hxx"

class AiNode;

class Behavior : public GenericComponent<Behavior, ComponentType::Behavior> {
public:
    Behavior() = default;
    Behavior(const Behavior& other);
    Behavior(Behavior&& other);

    Behavior& operator=(Behavior other);

    friend void swap(Behavior& lhs, Behavior& rhs) {
        using std::swap;
        swap(lhs.root_, rhs.root_);
        swap(lhs.ai_context_, rhs.ai_context_);
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
     * @briefs Assumes control of the given entity, resetting the current behavior state
     */
    void control(Entity* entity);

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

    AiNode* root() const;
protected:
    void on_owner_updated() override;
private:
    std::unique_ptr<AiNode> root_;
    AiContext ai_context_;
};

#endif