#ifndef JARALYN_SCENE_COMPOSER_HXX
#define JARALYN_SCENE_COMPOSER_HXX

#include "scene_writer.hxx"
#include "null_scene.hxx"
#include "world_scene.hxx"
#include "status_scene.hxx"

/**
 * @brief Scene stack manager
 */
class j_scene_composer : public j_scene_writer {
private:
    entt::dispatcher* const dispatcher_;

    std::vector<std::unique_ptr<j_scene>> scenes_;

    bool stack_update_ { false };
public:
    explicit j_scene_composer(entt::dispatcher* const dispatcher);
    j_scene& active();

    j_scene* load(j_scene_type type) override;
    void unload(j_id id) override;

    void render(j_display& display);
    void update(j_input_state& input);
};

#endif
