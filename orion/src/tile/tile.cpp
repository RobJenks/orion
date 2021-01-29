#include "tile.h"

namespace Orion
{
    Tile::Tile(DefId def, Dir4 rotation, Vec2<Coord> location)
		:
		m_definition(def),
		m_rotation(rotation),
		m_location(location)
    {
    }
}
