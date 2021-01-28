#pragma once

#include <optional>
#include "../math/vec2.h"
#include "dir8.h"

namespace Orion
{
	template <typename TCoord>
	class Grid
	{
	public:

		static const Vec2<TCoord> NO_CELL;
		static const size_t NO_INDEX;

		Grid(Vec2<TCoord> size);

		size_t getIndex(Vec2<TCoord> location) const;
		Vec2<TCoord> getLocation(size_t index) const;

		bool isValidLocation(Vec2<TCoord> location) const;

		// Returns the neighbouring cell, or NO_CELL if it is not a valid location
		Vec2<TCoord> getNeighbour(Vec2<TCoord> location, Dir8 direction) const;

		// Returns the neighbouring cell, or optional::empty if it is not a valid location
		std::optional<Vec2<TCoord>> getNeighbourOpt(Vec2<TCoord> location, Dir8 direction) const;

		// Returns the index of the next cell in this direction.  No bounds checking - assumes the next cell is a valid location
		size_t getNeighbourIndex(size_t index, Dir8 direction) const;

		// Returns the number of cells that exist from the given location in the given direction.  Assumes current location is valid
		size_t getCellCountInDirection(Vec2<TCoord> index, Dir8 direction) const;
		size_t getCellCountInDirection(size_t index, Dir8 direction) const;

		size_t getCellCountUpFrom(Vec2<TCoord> location) const;
		size_t getCellCountRightFrom(Vec2<TCoord> location) const;
		size_t getCellCountDownFrom(Vec2<TCoord> location) const;
		size_t getCellCountLeftFrom(Vec2<TCoord> location) const;

	private:
		Vec2<TCoord> m_size;

	};


	// ==================================================================================

	template <typename TCoord>
	const Vec2<TCoord> Grid<TCoord>::NO_CELL = Vec2<TCoord>(-1, -1);

	template <typename TCoord>
	const size_t Grid<TCoord>::NO_INDEX = std::numeric_limits<size_t>::max();

	template <typename TCoord>
	Grid<TCoord>::Grid(Vec2<TCoord> size)
		:
		m_size(size)
	{
		ASS(size.x > 0 && size.y > 0, "Invalid grid size " << size);
	}

	template <typename TCoord>
	size_t Grid<TCoord>::getIndex(Vec2<TCoord> location) const
	{
		ASS(location.x > 0 && location.y > 0, "Invalid location " << location);
		return size_t(location.x) + (size_t(location.y) * size_t(m_size.y));
	}

	template <typename TCoord>
	Vec2<TCoord> Grid<TCoord>::getLocation(size_t index) const
	{
		return Vec2<TCoord>(TCoord(index % m_size.x), TCoord(index / m_size.x));
	}

	template <typename TCoord>
	bool Grid<TCoord>::isValidLocation(Vec2<TCoord> location) const
	{
		return (location.x >= 0 && location.x < m_size.x&&
			location.y >= 0 && location.y < m_size.y);
	}

	// Returns the neighbouring cell, or NO_CELL if it is not a valid location
	template <typename TCoord>
	Vec2<TCoord> Grid<TCoord>::getNeighbour(Vec2<TCoord> location, Dir8 direction) const
	{
		auto neighbour = Direction::getNeighbour(location, direction);
		return (isValidLocation(neighbour) ? neighbour : NO_CELL);
	}

	// Returns the neighbouring cell, or optional::empty if it is not a valid location
	template <typename TCoord>
	std::optional<Vec2<TCoord>> Grid<TCoord>::getNeighbourOpt(Vec2<TCoord> location, Dir8 direction) const
	{
		auto neighbour = Direction::getNeighbour(location, direction);
		return (isValidLocation(neighbour) ? std::optional<Vec2<TCoord>>(neighbour) : std::optional<Vec2<TCoord>>());
	}

	// Returns the index of the next cell in this direction.  No bounds checking - assumes the next cell is a valid location
	template <typename TCoord>
	size_t Grid<TCoord>::getNeighbourIndex(size_t index, Dir8 direction) const
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

		ASS(false, "Invalid direction provided: " << (int)direction);
		return NO_INDEX;
	}

	// Returns the number of cells that exist from the given location in the given direction.  Assumes current location is valid
	template <typename TCoord>
	size_t Grid<TCoord>::getCellCountInDirection(size_t index, Dir8 direction) const
	{
		return getCellCountInDirection(getLocation(index), direction);
	}

	// Returns the number of cells that exist from the given location in the given direction.  Assumes current location is valid
	template <typename TCoord>
	size_t Grid<TCoord>::getCellCountInDirection(Vec2<TCoord> location, Dir8 direction) const
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

		ASS(false, "Invalid direction provided: " << (int)direction);
		return NO_INDEX;
	}

	template <typename TCoord>
	size_t Grid<TCoord>::getCellCountUpFrom(Vec2<TCoord> location) const
	{
		return location.y;
	}

	template <typename TCoord>
	size_t Grid<TCoord>::getCellCountRightFrom(Vec2<TCoord> location) const
	{
		return m_size.x - size_t(location.x) - 1;
	}

	template <typename TCoord>
	size_t Grid<TCoord>::getCellCountDownFrom(Vec2<TCoord> location) const
	{
		return m_size.y - size_t(location.y) - 1;
	}

	template <typename TCoord>
	size_t Grid<TCoord>::getCellCountLeftFrom(Vec2<TCoord> location) const
	{
		return location.x;
	}

}
