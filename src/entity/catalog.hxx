#ifndef JARALYN_CATALOG_HXX
#define JARALYN_CATALOG_HXX

#include "archetype.hxx"

class Catalog {
public:
    Catalog() = delete;

    /**
     * @brief Creates and takes ownership of an empty Archetype with the given name.
     *
     * You may modify the returned Archetype further using the returned pointer.
     * Returns nullptr if an Archetype with the given name already exists.
     */
    static Archetype* create_archetype(std::string_view name);

    /**
     * @brief Returns an Archetype by name or nullptr if the Archetype does not exist.
     */
    static const Archetype* archetype(std::string_view name);

    /**
     * @brief Removes all archetypes from the catalog
     */
    static void clear_archetypes();
protected:
    static inline std::unordered_map<std::string, std::unique_ptr<Archetype>> archetypes_;
};

#endif
