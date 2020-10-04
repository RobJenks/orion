#pragma once

#include <vector>
#include "render_config.h"

namespace Orion
{
	template <typename T>
	class RenderSlot
	{
	public:
		static const int DEFAULT_INSTANCE_ALLOC_SIZE = 8;

		RenderSlot(RenderConfig config);

		RenderSlot(RenderConfig config, T && first_instance);
		RenderSlot(RenderConfig config, const T & first_instance);

	private:

		RenderConfig m_config;
		std::vector<T> m_instances;

	};


	template<typename T>
	inline RenderSlot<T>::RenderSlot(RenderConfig config)
		:
		m_config(config)
	{
		m_instances.reserve(DEFAULT_INSTANCE_ALLOC_SIZE);
	}

	template<typename T>
	inline RenderSlot<T>::RenderSlot(RenderConfig config, T&& first_instance)
		:
		m_config(config),
		m_instances({ first_instance })
	{
		m_instances.reserve(DEFAULT_INSTANCE_ALLOC_SIZE);
	}

	template<typename T>
	inline RenderSlot<T>::RenderSlot(RenderConfig config, const T& first_instance)
		:
		m_config(config),
		m_instances({ first_instance })
	{
		m_instances.reserve(DEFAULT_INSTANCE_ALLOC_SIZE);
	}
}
