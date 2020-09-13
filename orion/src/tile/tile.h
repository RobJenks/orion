#pragma once

#include <stdint.h>
#include "../grid/dir4.h"

namespace Orion
{
	class Tile
	{
	public:



	private:

		uint32_t m_definition;
		Dir4 m_rotation;
		size_t m_location;

	};
}
