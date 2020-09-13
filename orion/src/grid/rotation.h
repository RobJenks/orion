#pragma once

#include <array>
class Dir4;
class Rot90;

namespace Orion
{
	class Rotation
	{
	public:

		static Dir4 rotate(Dir4 direction, Rot90 rotation);


	private:
		static const std::array<std::array<Dir4, 4>, 4> m_dir4_rotation;
	};
}
