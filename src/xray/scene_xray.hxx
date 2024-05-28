#ifndef JARALYN_SCENE_XRAY_HXX
#define JARALYN_SCENE_XRAY_HXX

#include "xray_interface.hxx"
#include "../input/input.hxx"
#include "../input/input_event.hxx"
#include "../scene/scene.hxx"
#include "../gfx/renderer.hxx"
#include "../entity/components/skills.hxx"

class SceneXray : public IXray {
public:
    SceneXray(EventManager* events);

    void update() override;
private:
    void entity_panel(std::optional<u64> entity_id);
    void entity_glyph(Entity* entity);

    struct TileWindowData {
        bool is_editing = false;
        TileType lmb_type = TileType::None;
        TileType rmb_type = TileType::None;
    } tile_window_data_;

    void entity_window();
    void tile_window();

    bool on_mouse_down(MouseDownEvent& e);
    bool on_config_updated(ConfigUpdatedEvent& e);

    Config _config;
};

#endif
