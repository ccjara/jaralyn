#ifndef JARALYN_VISION_HXX
#define JARALYN_VISION_HXX

#include "component/generic_component.hxx"
#include "component/vision/fov.hxx"

class ITileReader;

/**
 * @brief Represents the vision of an entity. Any entity with this component can see.
 */
class Vision : public GenericComponent<Vision, ComponentType::Vision> {
public:
    explicit Vision(ITileReader* tile_reader);

    /**
     * @brief Provides readonly access to the field of view
     */
    const Fov& fov() const;

    /**
     * @brief Sets the vision radius of the entity
     */
    void set_vision_radius(i32 radius);

    /**
     * @brief Returns the vision radius of the entity
     */
    i32 vision_radius() const;

    /**
     * @brief Returns whether the field of view should be applied to the map
     */
    bool applied() const;

    /**
     * @brief Sets whether the field of view should be applied to the map
     */
    void set_apply(bool apply_vision);

    void on_after_actions() override;
    void on_player_attached() override;
    void on_player_detached() override;

    void update(u64 dt) override;
private:
    Fov fov_;

    /**
     * @brief Radius used to calculate the field of view
     */
    i32 vision_radius_ = 0;

    /**
     * @brief Whether the FoV should be applied to the map
     */
    bool apply_ = false;

    void update_fov();

    ITileReader* tile_reader_ = nullptr;
};

#endif