#ifndef JARALYN_GENERIC_AI_NODE_HXX
#define JARALYN_GENERIC_AI_NODE_HXX

#include "ai/ai_node.hxx"

template<typename Derived>
class GenericAiNode : public AiNode {
public:
    [[nodiscard]] std::unique_ptr<AiNode> clone() const override {
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
};

#endif
