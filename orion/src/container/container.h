#pragma once

#include <vector>
#include <limits>

#include "../util/debug.h"
#include "../util/type_defaults.h"
#include "../grid/grid.h"
#include "../grid/quadtree.h"
#include "../tile/tile.h"

namespace Orion
{
	class Container
	{
	public:
		typedef size_t Index;
		typedef int Coord;
		typedef Grid<Index, Coord> TileGrid;
		static const Index NO_INDEX = std::numeric_limits<Index>::max();

		Container(Vec2<Coord> size);

		inline Vec2<Coord> getSize() const { return m_size; }

		const Tile::Collection& getTiles() const { return m_tiles; }
		const Tile * getTileAt(Vec2<Coord> location) const;
		const Tile & getTileAtUnchecked(Vec2<Coord> location) const;
		bool addTile(const Tile& tile);
		void addTileUnchecked(const Tile& tile);
		bool removeTileAt(Vec2<Coord> location);
		void removeTileAtUnchecked(Vec2<Coord> location);

	private:

		Index addTileAtNextFreeIndex(const Tile & tile);
		void moveTileIndexToFreeList(Index index);

	private:

		Vec2<Coord>				m_size;
		Index					m_count;

		TileGrid				m_grid;
		Tile::Collection		m_tiles;
		std::vector<Index>		m_free_tile_indices;

	};

}
