#ifndef NOEDEN_TILE_ACCESSOR_HXX
#define NOEDEN_TILE_ACCESSOR_HXX

class ChunkManager;
struct Tile;

class TileAccessor {
public:
    explicit TileAccessor(ChunkManager* chunk_manager);

    Tile* get_tile(const WorldPos& position);
    void set_tile(const WorldPos& position, const Tile& tile);
private:
    ChunkManager* chunk_manager_ = nullptr;

    size_t to_index(const WorldPos& position) const;
};

#endif
