#pragma once

#include <vector>

#include "../util/debug.h"
#include "../grid/grid.h"
#include "../grid/quadtree.h"
#include "../tile/tile.h"

namespace Orion
{
	template <typename T>
	class Container
	{
	public:
		typedef std::vector<T> Items;
		typedef typename Items::size_type Index;
		typedef int Coord;

		Container(Vec2<Coord> size);

		inline Vec2<Coord> getSize() const { return m_size; }

	private:

		static Vec2<Coord> validateSize(Vec2<Coord> size);

	private:

		Vec2<Coord>				m_size;
		Grid<Coord>				m_grid;
		Quadtree<Index, Coord>	m_tree;
		Items					m_items;

	};

	template <typename T>
	Container<T>::Container(Vec2<Coord> size)
		:
		m_size(Container<T>::validateSize(size)),
		m_grid(size),
		m_tree(Vec2<Coord>(0, 0), size),
		m_items()
	{
	}

	template <typename T>
	Vec2<typename Container<T>::Coord> Container<T>::validateSize(Vec2<Coord> size)
	{
		ASS(size.x > 0 && size.y > 0 &&
			size.x < 1000000 && size.y < 1000000, "Invalid container size " << size);

		return size;
	}
}
