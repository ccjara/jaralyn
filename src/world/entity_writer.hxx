#ifndef JARALYN_ENTITY_WRITER_HXX
#define JARALYN_ENTITY_WRITER_HXX

class Entity;
struct Archetype;

class IEntityWriter {
public:
    /**
     * @brief Creates an Entity of the given archetype and returns it.
     *
     * The Entity can be further configured after creation.
     */
    virtual Entity& create_entity(const Archetype& archetype) = 0;

    virtual ~IEntityWriter() = default;
};

#endif
