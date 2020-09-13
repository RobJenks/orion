#include <array>
#include <assert.h>
#include "dir4.h"
#include "dir8.h"

#include "direction.h"

namespace Orion
{
	const std::array<Vec2<int>, 4> Direction::m_offsets_4 = { {
		{ 0, 1 },	// Dir4::UP
		{ 1, 0 },	// Dir4::RIGHT
		{ 0, -1 },	// Dir4::DOWN
		{ -1, 0 }	// Dir4::LEFT
	} };

	const std::array<Vec2<int>, 8> Direction::m_offsets_8 = { {
		{ 0, 1 },	// Dir8::UP
		{ 1, 1 },	// Dir8::UP_RIGHT
		{ 1, 0 },	// Dir8::RIGHT
		{ 1, -1 },	// Dir8::DOWN_RIGHT
		{ 0, -1 },	// Dir8::DOWN
		{ -1, -1 },	// Dir8::DOWN_LEFT
		{ -1, 0 },	// Dir8::LEFT
		{ -1, 1 }	// Dir8::UP_LEFT
	} };

	const std::array<Dir8, 4> Direction::m_dir4_to_dir8 = {
		Dir8(static_cast<int>(Dir8::UP)),		// Dir4::UP
		Dir8(static_cast<int>(Dir8::RIGHT)),	// Dir4::RIGHT
		Dir8(static_cast<int>(Dir8::DOWN)),		// Dir4::DOWN
		Dir8(static_cast<int>(Dir8::LEFT))		// Dir4::LEFT
	};


	Vec2<int> Direction::getNeighbour(Vec2<int> location, Dir4 direction)
	{
		return (location + m_offsets_4[static_cast<int>(direction)]);
	}

	Vec2<int> Direction::getNeighbour(int x, int y, Dir4 direction)
	{
		auto neighbour = m_offsets_4[static_cast<int>(direction)];
		neighbour.x += x;
		neighbour.y += y;

		return neighbour;
	}

	Vec2<int> Direction::getNeighbour(Vec2<int> location, Dir8 direction)
	{
		return (location + m_offsets_8[static_cast<int>(direction)]);
	}
	
	Vec2<int> Direction::getNeighbour(int x, int y, Dir8 direction)
	{
		auto neighbour = m_offsets_8[static_cast<int>(direction)];
		neighbour.x += x;
		neighbour.y += y;

		return neighbour;
	}

	Dir8 Direction::convertDir4ToDir8(Dir4 dir)
	{
		return m_dir4_to_dir8[static_cast<int>(dir)];
	}


	static_assert(
		(int)Dir4::UP == 0 &&
		(int)Dir4::RIGHT == 1 &&
		(int)Dir4::DOWN == 2 &&
		(int)Dir4::LEFT == 3
	);

	static_assert(
		(int)Dir8::UP == 0 &&
		(int)Dir8::UP_RIGHT == 1 &&
		(int)Dir8::RIGHT == 2 &&
		(int)Dir8::DOWN_RIGHT == 3 &&
		(int)Dir8::DOWN == 4 &&
		(int)Dir8::DOWN_LEFT == 5 &&
		(int)Dir8::LEFT == 6 &&
		(int)Dir8::UP_LEFT == 7
	);

}
