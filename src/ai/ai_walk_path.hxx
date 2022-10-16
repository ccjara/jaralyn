#ifndef JARALYN_AI_WALK_PATH_HXX
#define JARALYN_AI_WALK_PATH_HXX

#include "ai_trace_path.hxx"

class AiWalkPath : public GenericAiNode<AiWalkPath> {
public:
    void clear() override;

    AiNodeState visit(AiContext& context) override;
private:
    AStarSearch search_;
};

#endif
