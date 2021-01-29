#pragma once

#include <vector>
#include "../util/debug.h"
#include "../math/vec2.h"
#include "dir8.h"

namespace Orion
{
	template <typename T, typename TCoord>
	class Grid
	{
	public:

		typedef std::vector<T> Data;
		typedef typename Data::size_type Index;
		static const Vec2<TCoord> NO_CELL;
		static const Index NO_INDEX;

		Grid(Vec2<TCoord> size, T initial, T defaultValue);

		Index getIndex(Vec2<TCoord> location) const;
		Index getIndex(TCoord x, TCoord y) const;
		Index getIndexUnchecked(Vec2<TCoord> location) const;
		Index getIndexUnchecked(TCoord x, TCoord y) const;

		Vec2<TCoord> getLocation(Index index) const;
		Vec2<TCoord> getLocationUnchecked(Index index) const;

		inline Vec2<TCoord> getSize() const { return m_size; }
		bool isValidLocation(Vec2<TCoord> location) const;
		bool isValidLocation(TCoord x, TCoord y) const;
		bool isValidIndex(Index index) const;

		T get(Index index) const;
		T get(Vec2<TCoord> location) const;

		T getOr(Index index, T defaultValue) const;
		T getOr(Vec2<TCoord> location, T defaultValue) const;
		
		T getOrDefault(Index index) const;
		T getOrDefault(Vec2<TCoord> location) const;
		
		T& getRefUnchecked(Index index);
		T& getRefUnchecked(Vec2<TCoord> location);

		void set(Index index, T&& value);
		void set(Index index, const T & value);
		void set(Vec2<TCoord> location, T&& value);
		void set(Vec2<TCoord> location, const T & value);
        
		bool setIfValidIndex(Index index, T&& value);
		bool setIfValidIndex(Index index, const T & value);
		bool setIfValidLocation(Vec2<TCoord> location, T&& value);
		bool setIfValidLocation(Vec2<TCoord> location, const T & value);

        T setAndReturnPrevious(Index index, T&& value);
        T setAndReturnPrevious(Index index, const T & value);
        T setAndReturnPrevious(Vec2<TCoord> location, T&& value);
        T setAndReturnPrevious(Vec2<TCoord> location, const T & value);

		bool isEdge(Index index) const;
		bool isEdgeUnchecked(Index index) const;
		bool isEdge(Vec2<TCoord> location) const;

		// Returns the neighbouring cell, or NO_CELL if it is not a valid location
		Vec2<TCoord> getNeighbour(Vec2<TCoord> location, Dir8 direction) const;
		Vec2<TCoord> getNeighbourUnchecked(Vec2<TCoord> location, Dir8 direction) const;

		// Returns the index of the next cell in this direction.  No bounds checking - assumes the next cell is a valid location
		Index getNeighbourIndexUnchecked(Index index, Dir8 direction) const;

        // Returns the value in the neighbouring cell
        T getAdjacentOr(Vec2<TCoord> location, Dir8 direction, T defaultValue) const;
        T getAdjacentUnchecked(Vec2<TCoord> location, Dir8 direction) const;

        // Returns the value of the next cell in this direction.  No bounds checking - assumes the next cell is a valid location
        T getAdjacentUnchecked(Index index, Dir8 direction) const;


		// Returns the number of cells that exist from the given location in the given direction.  Assumes current location is valid
		Index getCellCountInDirection(Vec2<TCoord> index, Dir8 direction) const;
		Index getCellCountInDirection(Index index, Dir8 direction) const;

		Index getCellCountUpFrom(Vec2<TCoord> location) const;
		Index getCellCountRightFrom(Vec2<TCoord> location) const;
		Index getCellCountDownFrom(Vec2<TCoord> location) const;
		Index getCellCountLeftFrom(Vec2<TCoord> location) const;

	private:

		static const TCoord MAX_DIMENSION  = 1000000;	// Max 1m in one dimension
		static const TCoord MAX_TOTAL_SIZE = 100000000;	// Max 100m total elements

		static Vec2<TCoord> validatedSize(Vec2<TCoord> size);
		static Index validatedCount(Index count);


	private:
		Vec2<TCoord>	m_size;
		Index			m_count;
		Data			m_data;
        T               m_default;
	};


	// ==================================================================================

	template <typename T, typename TCoord>
	const Vec2<TCoord> Grid<T, TCoord>::NO_CELL = Vec2<TCoord>(std::numeric_limits<TCoord>::max(), std::numeric_limits<TCoord>::max());

	template <typename T, typename TCoord>
	const typename Grid<T, TCoord>::Index Grid<T, TCoord>::NO_INDEX = std::numeric_limits<Index>::max();

	template <typename T, typename TCoord>
	Grid<T, TCoord>::Grid(Vec2<TCoord> size, T initial, T defaultValue)
		:
		m_size(validatedSize(size)),
		m_count(validatedCount(size.x * size.y)),
        m_default(defaultValue)
	{
		m_data = Data(m_count, initial);
	}

	template <typename T, typename TCoord>
	Vec2<TCoord> Grid<T, TCoord>::validatedSize(Vec2<TCoord> size)
	{
		ASS(size.x > 0 && size.y > 0 &&
			size.x < MAX_DIMENSION&& size.y < MAX_DIMENSION, "Invalid grid size " << size);

		return size;
	}

	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::validatedCount(Index count)
	{
		ASS(count > 0 && count < MAX_TOTAL_SIZE, "Invalid grid element count " << count);

		return count;
	}

	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getIndex(Vec2<TCoord> location) const
	{
		return isValidLocation(location) ? getIndexUnchecked(location) : NO_INDEX;
	}
	
	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getIndex(TCoord x, TCoord y) const
	{
		return isValidLocation(x, y) ? getIndexUnchecked(x, y) : NO_INDEX;
	}
	
	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getIndexUnchecked(Vec2<TCoord> location) const
	{
		ASS(location.x > 0 && location.y > 0, "Invalid location " << location);
		return Index(location.x) + (Index(location.y) * Index(m_size.y));
	}
	
	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getIndexUnchecked(TCoord x, TCoord y) const
	{
		ASS(x > 0 && y > 0, "Invalid location (" << x << "," << y << ")");
		return Index(x) + (Index(y) * Index(m_size.y));
	}

	template <typename T, typename TCoord>
	Vec2<TCoord> Grid<T, TCoord>::getLocation(Index index) const
	{
        return isValidIndex(index) ? getLocationUnchecked(index) : NO_CELL;
	}

	template <typename T, typename TCoord>
	Vec2<TCoord> Grid<T, TCoord>::getLocationUnchecked(Index index) const
	{
		return Vec2<TCoord>(TCoord(index % m_size.x), TCoord(index / m_size.x));
	}

	template <typename T, typename TCoord>
	bool Grid<T, TCoord>::isValidLocation(Vec2<TCoord> location) const
	{
		return (location.x >= 0 && location.x < m_size.x &&
			    location.y >= 0 && location.y < m_size.y);
	}
	
	template <typename T, typename TCoord>
	bool Grid<T, TCoord>::isValidLocation(TCoord x, TCoord y) const
	{
		return (x >= 0 && x < m_size.x && y >= 0 && y < m_size.y);
	}
	
	template <typename T, typename TCoord>
	bool Grid<T, TCoord>::isValidIndex(Index index) const
	{
		return (index >= 0 && index < m_count);
	}

	template<typename T, typename TCoord>
	inline T Grid<T, TCoord>::get(Index index) const
	{
        return m_data[index];
	}

	template<typename T, typename TCoord>
	inline T Grid<T, TCoord>::get(Vec2<TCoord> location) const
	{
        return get(getIndexUnchecked(std::move(location)));
	}

    template<typename T, typename TCoord>
    inline T Grid<T, TCoord>::getOr(Index index, T defaultValue) const
    {
        return isValidIndex(index) ? get(index) : defaultValue;
    }

	template<typename T, typename TCoord>
	inline T Grid<T, TCoord>::getOr(Vec2<TCoord> location, T defaultValue) const
	{
        const auto index = getIndex(location);
        return (index != NO_INDEX) ? get(index) : defaultValue;
	}

    template<typename T, typename TCoord>
    inline T Grid<T, TCoord>::getOrDefault(Index index) const
    {
        return isValidIndex(index) ? get(index) : m_default;
    }

    template<typename T, typename TCoord>
    inline T Grid<T, TCoord>::getOrDefault(Vec2<TCoord> location) const
    {
        const auto index = getIndex(location);
        return (index != NO_INDEX) ? get(index) : m_default;
    }

	template<typename T, typename TCoord>
	inline T& Grid<T, TCoord>::getRefUnchecked(Index index)
	{
        return m_data[index];
	}

	template<typename T, typename TCoord>
	inline T& Grid<T, TCoord>::getRefUnchecked(Vec2<TCoord> location)
	{
        return m_data[getIndex(location)];
	}

	template<typename T, typename TCoord>
	inline void Grid<T, TCoord>::set(Index index, T&& value)
	{
        m_data[index] = value;
	}

    template<typename T, typename TCoord>
    inline void Grid<T, TCoord>::set(Index index, const T& value)
    {
        m_data[index] = value;
    }

	template<typename T, typename TCoord>
	inline void Grid<T, TCoord>::set(Vec2<TCoord> location, T&& value)
	{
        m_data[getIndexUnchecked(location)] = value;
	}

    template<typename T, typename TCoord>
    inline void Grid<T, TCoord>::set(Vec2<TCoord> location, const T& value)
    {
        m_data[getIndexUnchecked(location)] = value;
    }

    template<typename T, typename TCoord>
    inline bool Grid<T, TCoord>::setIfValidIndex(Index index, T&& value)
    {
        if (!isValidIndex(index)) return false;

        set(index, value);
        return true;
    }

    template<typename T, typename TCoord>
    inline bool Grid<T, TCoord>::setIfValidIndex(Index index, const T& value)
    {
        if (!isValidIndex(index)) return false;

        set(index, value);
        return true;
    }

    template<typename T, typename TCoord>
    inline bool Grid<T, TCoord>::setIfValidLocation(Vec2<TCoord> location, T&& value)
    {
        if (!isValidLocation(location)) return false;

        set(location, value);
        return true;
    }

    template<typename T, typename TCoord>
    inline bool Grid<T, TCoord>::setIfValidLocation(Vec2<TCoord> location, const T& value)
    {
        if (!isValidLocation(location)) return false;

        set(location, value);
        return true;
    }

    template<typename T, typename TCoord>
    inline T Grid<T, TCoord>::setAndReturnPrevious(Index index, T&& value)
    {
        auto prev = get(index);
        set(index, value);
        return prev;
    }

    template<typename T, typename TCoord>
    inline T Grid<T, TCoord>::setAndReturnPrevious(Index index, const T& value)
    {
        auto prev = get(index);
        set(index, value);
        return prev;
    }

    template<typename T, typename TCoord>
    inline T Grid<T, TCoord>::setAndReturnPrevious(Vec2<TCoord> location, T&& value)
    {
        return setAndReturnPrevious(getIndexUnchecked(location), value);
    }

    template<typename T, typename TCoord>
    inline T Grid<T, TCoord>::setAndReturnPrevious(Vec2<TCoord> location, const T& value)
    {
        return setAndReturnPrevious(getIndexUnchecked(location), value);
    }

	template<typename T, typename TCoord>
	inline bool Grid<T, TCoord>::isEdge(Index index) const
	{
        return isValidIndex(index) && isEdgeUnchecked(index);
	}

	template<typename T, typename TCoord>
	inline bool Grid<T, TCoord>::isEdgeUnchecked(Index index) const
	{
        if (index < m_size.x || index >= (m_count - m_size.x)) return true;     // Top/bottom edge

        const auto col = index % m_size.x;
        return (col == 0 || col == m_size.x - 1);   // Left/right edge
	}

    template<typename T, typename TCoord>
    inline bool Grid<T, TCoord>::isEdge(Vec2<TCoord> location) const
    {
        return location.x == 0 || location.x == m_size.x - 1 ||
               location.y == 0 || location.y == m_size.y - 1;
    }

	// Returns the neighbouring cell, or optional::empty if it is not a valid location
	template <typename T, typename TCoord>
	Vec2<TCoord> Grid<T, TCoord>::getNeighbour(Vec2<TCoord> location, Dir8 direction) const
	{
		const auto neighbour = getNeighbourUnchecked(location, direction);
		return (isValidLocation(neighbour) ? neighbour : NO_CELL);
	}

	// Returns the neighbouring cell, or NO_CELL if it is not a valid location
	template <typename T, typename TCoord>
	Vec2<TCoord> Grid<T, TCoord>::getNeighbourUnchecked(Vec2<TCoord> location, Dir8 direction) const
	{
		return Direction::getNeighbour(location, direction);
	}

	// Returns the index of the next cell in this direction.  No bounds checking - assumes the next cell is a valid location
	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getNeighbourIndexUnchecked(Index index, Dir8 direction) const
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

    template<typename T, typename TCoord>
    inline T Grid<T, TCoord>::getAdjacentOr(Vec2<TCoord> location, Dir8 direction, T defaultValue) const
    {
        const auto neighbour = getNeighbour(location, direction);
        return (neighbour != NO_CELL ? get(neighbour) : defaultValue);
    }

	template<typename T, typename TCoord>
	inline T Grid<T, TCoord>::getAdjacentUnchecked(Vec2<TCoord> location, Dir8 direction) const
	{
        return get(getNeighbour(location, direction));
	}

	template<typename T, typename TCoord>
	inline T Grid<T, TCoord>::getAdjacentUnchecked(Index index, Dir8 direction) const
	{
        return get(getNeighbourIndexUnchecked(index, direction));
	}

	// Returns the number of cells that exist from the given location in the given direction.  Assumes current location is valid
	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getCellCountInDirection(Index index, Dir8 direction) const
	{
		return getCellCountInDirection(getLocation(index), direction);
	}

	// Returns the number of cells that exist from the given location in the given direction.  Assumes current location is valid
	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getCellCountInDirection(Vec2<TCoord> location, Dir8 direction) const
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

	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getCellCountUpFrom(Vec2<TCoord> location) const
	{
		return location.y;
	}

	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getCellCountRightFrom(Vec2<TCoord> location) const
	{
		return m_size.x - Index(location.x) - 1;
	}

	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getCellCountDownFrom(Vec2<TCoord> location) const
	{
		return m_size.y - Index(location.y) - 1;
	}

	template <typename T, typename TCoord>
	typename Grid<T, TCoord>::Index Grid<T, TCoord>::getCellCountLeftFrom(Vec2<TCoord> location) const
	{
		return location.x;
	}

}
