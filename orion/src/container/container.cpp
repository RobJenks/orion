#include "container.h"

namespace Orion
{
	Container::Container()
		:
		m_grid(Vec2<int>(1,1)),
		m_tiles(),
		m_tree(Vec2<int>(0,0), Vec2<int>(10,10))
	{
	}

	int Container::tmp() {
		m_tree.addItem(tmps({ 1,2 }, 12));
		m_tree.addItem(tmps({ 4,4 }, 44));
		m_tree.addItem(tmps({ 8,8 }, 88));
		m_tree.addItem(tmps({ 6,6 }, 66));

		std::vector<tmps> vec;
		m_tree.findItems(Vec2<int>{0,0}, Vec2<int>{7,7}, vec);

		return int(vec.size());
	}
}
