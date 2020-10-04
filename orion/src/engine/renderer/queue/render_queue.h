#pragma once

#include <vector>
#include <unordered_map>
#include "render_slot.h"
#include "render_config.h"

namespace Orion
{
	class RenderQueue
	{
	public:

		

	private:

		std::vector<RenderSlot> m_slots;
		std::unordered_map<RenderConfig, size_t> m_slot_map;

	};
}
