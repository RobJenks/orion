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

		inline Vec2<int> getPosition() const { return pos; }
	};


	class Container
	{
	public:
		Container();
		int tmp();

	private:

		Grid				m_grid;
		Quadtree<tmps>	m_tree;
		std::vector<Tile>	m_tiles;


	};
}
