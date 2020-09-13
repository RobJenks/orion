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
		static Vec2<int> getNeighbour(int x, int y, Dir4 direction);

		static Vec2<int> getNeighbour(Vec2<int> location, Dir8 direction);
		static Vec2<int> getNeighbour(int x, int y, Dir8 direction);

		static Dir8 convertDir4ToDir8(Dir4 dir);

	private:

		static const std::array<Vec2<int>, 4> m_offsets_4;
		static const std::array<Vec2<int>, 8> m_offsets_8;

		static const std::array<Dir8, 4> m_dir4_to_dir8;
	};
}
