#include "container.h"

namespace Orion
{
	Container::Container()
		:
		m_grid(Vec2<int>(1,1)),
		m_tiles(),
		m_tree(Vec2<int>(-1,-1), Vec2<int>(1,1))
	{
	}

	int Container::tmp() {
		std::vector<tmps> vec;
		m_tree.findItems(Vec2<int>{-10, -10}, Vec2<int>{10, 10}, vec);

		return int(vec.size());
	}
}
