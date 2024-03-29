#ifndef JARALYN_AI_SCRIPT_HXX
#define JARALYN_AI_SCRIPT_HXX

#include "generic_ai_node.hxx"
#include "../scripts/scripting.hxx"
#include "../scene/scene.hxx"

class AiScript : public GenericAiNode<AiScript> {
public:
    void clear() override;

    AiNodeState visit(AiContext& context) override;
};

#endif
