#pragma once

#include "../math/vec2.h"

namespace Orion
{
	class Grid
	{
	public:

		Grid(int width, int height);

		size_t getIndex(int x, int y);
		size_t getIndex(Vec2<int> location);

		Vec2<int> getLocation(size_t index);


	private:
		Vec2<int> m_size;

	};
}
