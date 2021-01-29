#pragma once

#include <stdint.h>
#include <vector>
#include "../grid/dir4.h"

namespace Orion
{
	class Tile
	{
	public:

		typedef std::vector<Tile> Collection;


	private:

		uint32_t m_definition;
		Dir4 m_rotation;
		size_t m_location;

	};
}
