#ifndef JARALYN_CATALOG_API_HXX
#define JARALYN_CATALOG_API_HXX

#include "scripts/api/lua_api.hxx"

class AiNode;
class Archetype;
class Catalog;
class ServiceLocator;

class CatalogApi final : public LuaApi {
public:
    explicit CatalogApi(Catalog* catalog, ServiceLocator* services);

    void on_register(Script* script) final override;

    void create_archetype(luabridge::LuaRef ref);

    void clear_archetypes();
private:
    ServiceLocator* services_ = nullptr;
    Catalog* catalog_ = nullptr;

    std::unique_ptr<AiNode> create_behavior_node(const luabridge::LuaRef& ref);

    void add_behavior_component(Archetype& archetype, const luabridge::LuaRef& ref);
};

#endif
