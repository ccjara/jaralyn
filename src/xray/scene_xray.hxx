#ifndef JARALYN_SCENE_XRAY_HXX
#define JARALYN_SCENE_XRAY_HXX

#include "xray_interface.hxx"

#include "config/config.hxx"
#include "tile/tile.hxx"

class Entity;
class EntityManager;
class TileManager;
class IInputReader;

class ConfigUpdatedEvent;
class MouseDownEvent;

class SceneXray : public IXray {
public:
    explicit SceneXray(
        EntityManager* entity_manager,
        TileManager* tile_manager,
        EventManager* events,
        IInputReader* input
    );
    
    void update() override;
private:
    void entity_panel(std::optional<Id> entity_id);
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

    Config config_;

    EntityManager* entity_manager_ = nullptr;
    TileManager* tile_manager_ = nullptr;
    EventManager* events_ = nullptr;
    IInputReader* input_ = nullptr;
};

#endif
