#ifndef JARALYN_TRACE_PATH_HXX
#define JARALYN_TRACE_PATH_HXX

#include "../math/astar.hxx"
#include "generic_ai_node.hxx"

class AiTracePath : public GenericAiNode<AiTracePath> {
public:
    void clear() override;

    AiNodeState visit(AiContext& context) override;

    [[nodiscard]] const AStarSearch& search() const;
private:
    AStarSearch search_;
};


#endif
