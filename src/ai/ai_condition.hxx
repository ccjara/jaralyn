#ifndef JARALYN_AI_CONDITION_HXX
#define JARALYN_AI_CONDITION_HXX

#include "ai/generic_ai_node.hxx"

/**
 * @brief Condition node which runs a bool function to determine its state
 */
class AiCondition : public GenericAiNode<AiCondition> {
    using ConditionFn = std::function<bool(AiContext&)>;

public:
    explicit AiCondition(ConditionFn condition);

    AiNodeState visit(AiContext& context) override;

    void clear() override;
protected:
    ConditionFn condition_;
};

#endif