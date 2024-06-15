#ifndef NOEDEN_SCENE_XRAY_HXX
#define NOEDEN_SCENE_XRAY_HXX

#include "xray/xray_interface.hxx"
#include "config/config_event.hxx"
#include "config/config.hxx"
#include "input/input_event.hxx"
#include "tile/tile.hxx"
#include "xray/noise_texture.hxx"

class Entity;
class EntityManager;
class TileManager;
class IInputReader;
class ChunkManager;
class TileAccessor;
class WorldSpec;

class SceneXray : public IXray {
public:
    explicit SceneXray(
        WorldSpec* world_spec,
        ChunkManager* chunk_manager,
        EntityManager* entity_manager,
        TileAccessor* tile_accessor,
        TileManager* tile_manager,
        Events* events,
        IInputReader* input,
        Translator* translator
    );
    
    void update() override;
private:
    void entity_panel(std::optional<Id> entity_id);
    void entity_glyph(Entity* entity);

    void entity_window();
    void tile_window();
    void mapgen_window();

    EventResult on_mouse_down(const MouseDownEvent& e);
    Subscription<MouseDownEvent> mouse_down_sub_;
    EventResult on_config_updated(const ConfigUpdatedEvent& e);
    Subscription<ConfigUpdatedEvent> config_updated_sub_;

    Config config_;

    WorldSpec* world_spec_ = nullptr;
    ChunkManager* chunk_manager_ = nullptr;
    EntityManager* entity_manager_ = nullptr;
    TileAccessor* tile_accessor_ = nullptr;
    TileManager* tile_manager_ = nullptr;
    Events* events_ = nullptr;
    IInputReader* input_ = nullptr;
    Translator* translator_ = nullptr;

    NoiseTexture noise_texture_;
};

#endif
