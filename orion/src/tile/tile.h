#pragma once

#include <stdint.h>
#include <vector>
#include "../math/vec2.h"
#include "../grid/dir4.h"

namespace Orion
{
	class Tile
	{
	public:

		typedef uint32_t DefId;
		typedef int Coord;
		typedef std::vector<Tile> Collection;

		Tile(DefId def, Dir4 rotation, Vec2<Coord> location);

		inline DefId getDefinition() const { return m_definition; }
		inline Dir4 getRotation() const { return m_rotation; }
		inline Vec2<Coord> getLocation() const { return m_location; }



	private:

		DefId m_definition;
		Dir4 m_rotation;
		Vec2<Coord> m_location;

	};
}
