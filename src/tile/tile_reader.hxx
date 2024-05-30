#ifndef JARALYN_TILE_READER_HXX
#define JARALYN_TILE_READER_HXX

struct Tile;

class ITileReader {
public:
    virtual ~ITileReader() = default;

    /**
     * @brief Returns a mutable tile at the given position if existing
     * 
     * @param position Position of the tile to find
     * @return Tile* Target tile or nullptr if not found
     */
    virtual Tile* tile(Vec2<i32> position) = 0;

    /**
     * @brief Returns an immutable tile at the given position if existing
     * 
     * @param position Position of the tile to find
     * @return Tile* Target tile or nullptr if not found
     */
    virtual const Tile* tile(Vec2<i32> position) const = 0;

    /**
     * @brief Returns a reference to the tile container
     * 
     * @return Grid<Tile> const& Tile container
     */
    virtual Grid<Tile> const& tiles() const = 0;

    /**
     * @brief Returns a mutable reference to the tile container
     * 
     * @return Grid<Tile> const& Tile container
     */
    virtual Grid<Tile>& tiles() = 0;
};

#endif
