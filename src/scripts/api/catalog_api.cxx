#include "catalog_api.hxx"

void CatalogApi::on_register(Script* script) {
    luabridge::getGlobalNamespace(*script)
        .beginClass<CatalogApi>("Catalog")
            .addFunction("create_archetype", &CatalogApi::create_archetype)
            .addFunction("clear_archetypes", &CatalogApi::clear_archetypes)
        .endClass();

    expose(script, this, "catalog");
}

void CatalogApi::clear_archetypes() {
    Catalog::clear_archetypes();
}

void CatalogApi::create_archetype(luabridge::LuaRef ref) {
    if (!ref.isTable()) {
        Log::error("Expected archetype data to be a table");
        return;
    }
    if (!ref["name"].isString()) {
        Log::error("Archetype specification has no name");
        return;
    }
    auto archetype = Catalog::create_archetype(ref["name"].cast<const char*>());
    if (!archetype) {
        return;
    }
    const auto speed_ref = ref["speed"];
    if (speed_ref.isNumber()) {
        archetype->speed = speed_ref.cast<i32>();
    }
    const auto components_ref = ref["components"];
    if (components_ref.isTable()) {
        const auto length = components_ref.length();

        auto parse_components = [this, archetype, &ref](const luabridge::LuaRef& component_ref) -> void {
            if (component_ref.isNil()) {
                return;
            }
            const auto type_ref = component_ref["type"];
            if (!type_ref.isNumber()) {
                Log::error("Invalid {} component: type must be a string", archetype->name);
            }
            const auto component_type_unsafe = type_ref.cast<i32>();
            switch (static_cast<ComponentType>(component_type_unsafe)) {
                case ComponentType::Render: {
                    const auto glyph_ref = component_ref["glyph"];
                    auto component_ptr = new Render();
                    archetype->components.emplace_back(component_ptr);
                    auto& info = component_ptr->display_info();
                    if (!glyph_ref.isNumber()) {
                        Log::warn("Glyph in render component spec of {} is not a number", archetype->name);
                        return;
                    }
                    const auto color_ref = component_ref["color"];
                    info.glyph = glyph_ref.cast<i32>();
                    info.visible = true;

                    if (color_ref.isNumber()) {
                        info.color = Color(color_ref.cast<i32>());
                    }
                    return;
                }
                case ComponentType::Behavior: {
                    return add_behavior_component(*archetype, component_ref);
                }
                default:
                    Log::warn("Invalid {} component: unknown type id {}", archetype->name, component_type_unsafe);
            }
        };

        for (i32 nz_offset = 1U; nz_offset <= length; ++nz_offset) {
            parse_components(components_ref[nz_offset]);
        }
    }
}

AiNodeType parse_node_type(const luabridge::LuaRef& ref) {
    if (!ref.isNumber()) {
        return AiNodeType::None;
    }
    const auto unsafe_value = static_cast<AiNodeType>(ref.cast<i32>());

    switch (unsafe_value) {
        case AiNodeType::None:
        case AiNodeType::PrioritySelector:
        case AiNodeType::Walk:
            return unsafe_value;
        default:
            return AiNodeType::None;
    }
}

std::unique_ptr<AiNode> create_behavior_node(const luabridge::LuaRef& ref) {
    const auto type_ref = ref["type"];
    const auto type = parse_node_type(type_ref);

    std::unique_ptr<AiNode> base_node_ptr;

    switch (type) {
        case AiNodeType::PrioritySelector: {
            base_node_ptr = std::make_unique<AiPrioritySelector>();
            auto node_ptr = static_cast<AiPrioritySelector*>(base_node_ptr.get());

            const auto children_ref = ref["children"];
            if (!children_ref.isTable()) {
                break;
            }

            const auto children_length = children_ref.length();

            // for_each please!
            for (i32 i = 1; i <= children_length; ++i) {
                const auto priority_ref= ref["priority"];
                i32 priority = 0;
                if (priority_ref.isNumber()) {
                    priority = priority_ref.cast<i32>();
                } else {
                    Log::warn("Prioritized node must have a priority. Falling back to 0");
                }
                node_ptr->add(priority, create_behavior_node(children_ref[i]));
            }

            break;
        }
        case AiNodeType::Walk:
            base_node_ptr = std::make_unique<AiWalk>();
            break;
        default:
            Log::error("Unknown node type id {}", type);
    }
    return base_node_ptr;
}


void CatalogApi::add_behavior_component(Archetype& archetype, const luabridge::LuaRef& ref) {
    const auto root_ref = ref["root"];
    if (!root_ref.isTable()) {
        Log::error("Root in behavior component spec of {} is not a table.", archetype.name);
        return;
    }
    auto behavior = new Behavior();
    archetype.components.emplace_back(behavior);
    behavior->set_root(create_behavior_node(root_ref));
    if (!behavior->root()) {
        Log::warn("Behavior tree constructed without any nodes in archetype {}", archetype.name);
    }
}

AiNodeType CatalogApi::parse_node_type(const luabridge::LuaRef& ref) const {
    if (!ref.isNumber()) {
        return AiNodeType::None;
    }
    const auto unsafe_value = static_cast<AiNodeType>(ref.cast<i32>());

    switch (unsafe_value) {
        case AiNodeType::None:
        case AiNodeType::PrioritySelector:
        case AiNodeType::Walk:
            return unsafe_value;
        default:
            return AiNodeType::None;
    }
}
