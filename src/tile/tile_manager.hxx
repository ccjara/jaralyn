#ifndef JARALYN_TILE_MANAGER_HXX
#define JARALYN_TILE_MANAGER_HXX

#include "tile_reader.hxx"
#include "tile_writer.hxx"

class TileManager : public ITileReader, public ITileWriter {
public:
    TileManager();

    /**
     * @copydoc ITileWriter::create_tile
     */
    Tile& create_tile(TileType type, Vec2<i32> position) override;

    /**
     * @copydoc ITileReader::tile
     */
    Tile* tile(Vec2<i32> position) override;

    /**
     * @copydoc ITileReader::tile
     */
    const Tile* tile(Vec2<i32> position) const override;

    /**
     * @copydoc ITileReader::tiles
     */
    Grid<Tile> const& tiles() const override;

    /**
     * @copydoc ITileReader::tiles
     */
    Grid<Tile>& tiles() override;
private:
    /**
     * @brief Container of all tiles
     */
    Grid<Tile> tiles_;
};

#endif
