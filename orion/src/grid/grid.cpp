#include <assert.h>
#include "grid.h"

namespace Orion
{
	Grid::Grid(int width, int height)
		:
		m_size(width, height)
	{
		assert(width > 0 && height > 0);
	}

	size_t Grid::getIndex(int x, int y)
	{
		assert(x > 0 && y > 0);
		return size_t(x) + (size_t(y) * size_t(m_size.y));
	}

	size_t Grid::getIndex(Vec2<int> location)
	{
		return getIndex(location.x, location.y);
	}

	Vec2<int> Grid::getLocation(size_t index)
	{
		return Vec2<int>(int(index % m_size.x), int(index / m_size.x));
	}


}
