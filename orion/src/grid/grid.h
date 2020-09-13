#pragma once

#include <optional>
#include "../math/vec2.h"
#include "dir8.h"

namespace Orion
{
	class Grid
	{
	public:

		static const Vec2<int> NO_CELL;
		static const size_t NO_INDEX;

		Grid(Vec2<int> size);

		size_t getIndex(Vec2<int> location) const;
		Vec2<int> getLocation(size_t index) const;

		bool isValidLocation(Vec2<int> location) const;

		// Returns the neighbouring cell, or NO_CELL if it is not a valid location
		Vec2<int> getNeighbour(Vec2<int> location, Dir8 direction) const;

		// Returns the neighbouring cell, or optional::empty if it is not a valid location
		std::optional<Vec2<int>> getNeighbourOpt(Vec2<int> location, Dir8 direction) const;

		// Returns the index of the next cell in this direction.  No bounds checking - assumes the next cell is a valid location
		size_t getNeighbourIndex(size_t index, Dir8 direction) const;

		// Returns the number of cells that exist from the given location in the given direction.  Assumes current location is valid
		size_t getCellCountInDirection(Vec2<int> index, Dir8 direction) const;
		size_t getCellCountInDirection(size_t index, Dir8 direction) const;

		size_t getCellCountUpFrom(Vec2<int> location) const;
		size_t getCellCountRightFrom(Vec2<int> location) const;
		size_t getCellCountDownFrom(Vec2<int> location) const;
		size_t getCellCountLeftFrom(Vec2<int> location) const;

	private:
		Vec2<int> m_size;

	};
}
