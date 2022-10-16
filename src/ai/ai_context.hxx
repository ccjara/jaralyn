#ifndef JARALYN_AI_CONTEXT_HXX
#define JARALYN_AI_CONTEXT_HXX

struct AiContext {
    /**
     * @brief Id of the entity this Ai assumes control of
     */
    Id entity_id;

    // TODO: define "output node" for supporting nodes so they can pass blackboard refs
    // down to other components (ex. trace_path -> walk_path)
};

#endif
