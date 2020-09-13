#pragma once

#include <array>
#include "../math/vec2.h"
class Dir4;
class Dir8;

namespace Orion
{
	class Direction
	{
	public:

		static Vec2<int> getNeighbour(Vec2<int> location, Dir4 direction);
		static Vec2<int> getNeighbour(Vec2<int> location, Dir8 direction);

	private:

		static const std::array<Vec2<int>, 4> m_offsets_4;
		static const std::array<Vec2<int>, 8> m_offsets_8;

	};
}
