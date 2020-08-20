#ifndef JARALYN_INVENTORY_SCENE_HXX
#define JARALYN_INVENTORY_SCENE_HXX

#include "scene.hxx"
#include "../components/components.hxx"
#include "../gfx/gfx_system.hxx"

/**
 * @brief Renders an inventory of any game unit
 */
class j_inventory_scene : public j_scene {
protected:
    entt::entity entity_;
    entt::registry* registry_ { nullptr };
public:
    j_inventory_scene();

    void configure(entt::registry* registry, entt::entity entity);
    void update(j_input_state& input) override;
    void render(j_display& display) override;
};

#endif
