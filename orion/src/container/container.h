#pragma once

#include <vector>

#include "../grid/grid.h"
#include "../grid/quadtree.h"
#include "../tile/tile.h"

namespace Orion
{
	// Temporary
	struct tmps
	{
		Vec2<int> pos;
		size_t value;

		tmps(Vec2<int> _pos, size_t _value) : pos(_pos), value(_value) { }
		inline Vec2<int> getPosition() const { return pos; }
		inline size_t getValue() const { return value; }

	};


	class Container
	{
	public:
		Container();
		int tmp();

	private:

		Grid				m_grid;
		Quadtree<tmps, int>	m_tree;
		std::vector<Tile>	m_tiles;


	};
}
