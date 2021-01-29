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
	template <typename T>
	class Container
	{
	public:
		typedef size_t Index;
		typedef int Coord;
		static const Index NO_INDEX = std::numeric_limits<Index>::max();

		Container(Vec2<Coord> size);

		inline Vec2<Coord> getSize() const { return m_size; }


	private:

		Vec2<Coord>				m_size;
		Index					m_count;

		Grid<Index, Coord>		m_grid;
		Tile::Collection		m_tiles;

	};

	template <typename T>
	Container<T>::Container(Vec2<Coord> size)
		:
		m_size(size),
		m_count(size.x * size.y),
		m_grid(size, NO_INDEX)
	{
	}

}
