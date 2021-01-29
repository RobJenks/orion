#include "container.h"

namespace Orion
{
	Container::Container(Vec2<Coord> size)
		:
		m_size(size),
		m_count(Index(size.x) * Index(size.y)),
		m_grid(size, NO_INDEX, NO_INDEX)
	{
	}

    const Tile * Container::getTileAt(Vec2<Coord> location) const
	{
        const auto ix = m_grid.getOrDefault(location);
        return ix != NO_INDEX ? &(m_tiles[ix]) : nullptr;
	}

    const Tile & Container::getTileAtUnchecked(Vec2<Coord> location) const
    {
        return m_tiles[m_grid.get(location)];
    }

    bool Container::addTile(const Tile& tile)
    {
        auto grid_ix = m_grid.getIndex(tile.getLocation());
        if (grid_ix != TileGrid::NO_INDEX)
        {
            auto tile_ix = addTileAtNextFreeIndex(tile);
            m_grid.set(grid_ix, tile_ix);
            return true;
        }

        return false;
    }

    void Container::addTileUnchecked(const Tile& tile)
    {
        auto ix = addTileAtNextFreeIndex(tile);
        m_grid.set(tile.getLocation(), std::move(ix));
    }

    bool Container::removeTileAt(Vec2<Coord> location)
    {
        auto grid_ix = m_grid.getIndex(location);
        if (grid_ix == TileGrid::NO_INDEX) return false;

        auto tile_ix = m_grid.get(grid_ix);
        if (tile_ix == NO_INDEX) return false;

        // We DO NOT remove the tile from the tile collection; add to the free list instead
        moveTileIndexToFreeList(tile_ix);
        return true;
    }

    void Container::removeTileAtUnchecked(Vec2<Coord> location)
    {
        auto removed_tile_ix = m_grid.setAndReturnPrevious(location, NO_INDEX);
        moveTileIndexToFreeList(removed_tile_ix);
    }

    Container::Index Container::addTileAtNextFreeIndex(const Tile& tile)
    {
        if (m_free_tile_indices.empty())
        {
            auto ix = m_tiles.size();
            m_tiles.push_back(tile);
            return ix;
        }
        else
        {
            auto ix = m_free_tile_indices.back();
            m_free_tile_indices.pop_back();
            m_tiles[ix] = tile;
            return ix;
        }
    }

    void Container::moveTileIndexToFreeList(Index index)
    {
		m_free_tile_indices.push_back(index);
    }


}
