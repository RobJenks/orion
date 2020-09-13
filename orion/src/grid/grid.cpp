#include <assert.h>
#include "../util/types.h"
#include "grid.h"
#include "direction.h"

namespace Orion
{
	const Vec2<int> Grid::NO_CELL = Vec2<int>(-1, -1);
	const size_t Grid::NO_INDEX = std::numeric_limits<size_t>::max();

	Grid::Grid(Vec2<int> size)
		:
		m_size(size)
	{
		assert(size.x > 0 && size.y > 0);
	}

	size_t Grid::getIndex(Vec2<int> location) const
	{
		assert(location.x > 0 && location.y > 0);
		return size_t(location.x) + (size_t(location.y) * size_t(m_size.y));
	}

	Vec2<int> Grid::getLocation(size_t index) const
	{
		return Vec2<int>(int(index % m_size.x), int(index / m_size.x));
	}

	bool Grid::isValidLocation(Vec2<int> location) const
	{
		return (location.x >= 0 && location.x < m_size.x &&
			    location.y >= 0 && location.y < m_size.y);
	}

	// Returns the neighbouring cell, or NO_CELL if it is not a valid location
	Vec2<int> Grid::getNeighbour(Vec2<int> location, Dir8 direction) const
	{
		auto neighbour = Direction::getNeighbour(location, direction);
		return (isValidLocation(neighbour) ? neighbour : NO_CELL);
	}

	// Returns the neighbouring cell, or optional::empty if it is not a valid location
	std::optional<Vec2<int>> Grid::getNeighbourOpt(Vec2<int> location, Dir8 direction) const
	{
		auto neighbour = Direction::getNeighbour(location, direction);
		return (isValidLocation(neighbour) ? std::optional<Vec2<int>>(neighbour) : std::optional<Vec2<int>>());
	}

	// Returns the index of the next cell in this direction.  No bounds checking - assumes the next cell is a valid location
	size_t Grid::getNeighbourIndex(size_t index, Dir8 direction) const
	{
		switch (direction)
		{
			case Dir8::UP:			return index + m_size.y;
			case Dir8::UP_RIGHT:	return index + m_size.y + 1;
			case Dir8::RIGHT:		return index + 1;
			case Dir8::DOWN_RIGHT:	return index - m_size.y + 1;
			case Dir8::DOWN:		return index - m_size.y;
			case Dir8::DOWN_LEFT:	return index - m_size.y - 1;
			case Dir8::LEFT:		return index - 1;
			case Dir8::UP_LEFT:		return index + m_size.y - 1;
		}

		assert(false);
		return NO_INDEX;
	}

	// Returns the number of cells that exist from the given location in the given direction.  Assumes current location is valid
	size_t Grid::getCellCountInDirection(size_t index, Dir8 direction) const
	{
		return getCellCountInDirection(getLocation(index), direction);
	}

	// Returns the number of cells that exist from the given location in the given direction.  Assumes current location is valid
	size_t Grid::getCellCountInDirection(Vec2<int> location, Dir8 direction) const
	{
		switch (direction)
		{
			case Dir8::UP:			return			getCellCountUpFrom(location);
			case Dir8::UP_RIGHT:	return std::min(getCellCountUpFrom(location), getCellCountRightFrom(location));
			case Dir8::RIGHT:		return			getCellCountRightFrom(location);
			case Dir8::DOWN_RIGHT:	return std::min(getCellCountDownFrom(location), getCellCountRightFrom(location));
			case Dir8::DOWN:		return			getCellCountDownFrom(location);
			case Dir8::DOWN_LEFT:	return std::min(getCellCountDownFrom(location), getCellCountLeftFrom(location));
			case Dir8::LEFT:		return			getCellCountLeftFrom(location);
			case Dir8::UP_LEFT:		return std::min(getCellCountUpFrom(location), getCellCountLeftFrom(location));
		}

		assert(false);
		return NO_INDEX;
	}

	size_t Grid::getCellCountUpFrom(Vec2<int> location) const
	{
		return location.y;
	}

	size_t Grid::getCellCountRightFrom(Vec2<int> location) const
	{
		return m_size.x - size_t(location.x) - 1;
	}

	size_t Grid::getCellCountDownFrom(Vec2<int> location) const
	{
		return m_size.y - size_t(location.y) - 1;
	}

	size_t Grid::getCellCountLeftFrom(Vec2<int> location) const
	{
		return location.x;
	}

}
