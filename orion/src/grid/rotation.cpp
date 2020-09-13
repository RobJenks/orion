#include "dir4.h"
#include "rot90.h"
#include "rotation.h"

namespace Orion
{
	const std::array<std::array<Dir4, 4>, 4> Rotation::m_dir4_rotation = { {
		{ Dir4::UP, Dir4::RIGHT, Dir4::DOWN, Dir4::LEFT },
		{ Dir4::RIGHT, Dir4::DOWN, Dir4::LEFT, Dir4::UP },
		{ Dir4::DOWN, Dir4::LEFT, Dir4::UP, Dir4::RIGHT },
		{ Dir4::LEFT, Dir4::UP, Dir4::RIGHT, Dir4::DOWN }
	} };

	Dir4 Rotation::rotate(Dir4 direction, Rot90 rotation)
	{
		return m_dir4_rotation[static_cast<int>(direction)][static_cast<int>(rotation)];
	}



	static_assert(
		static_cast<int>(Rot90::Rot0) == 0 && 
		static_cast<int>(Rot90::Rot90) == 1 && 
		static_cast<int>(Rot90::Rot180) == 2 && 
		static_cast<int>(Rot90::Rot270) == 3 
	);

}
