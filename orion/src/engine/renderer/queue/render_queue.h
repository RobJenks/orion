#pragma once

#include <vector>
#include <unordered_map>
#include "render_slot.h"
#include "render_config.h"

namespace Orion
{
	template <typename T>
	class RenderQueue
	{
	public:
		RenderQueue();

		void submit(const RenderConfig& config, T&& instance);
		void submit(const RenderConfig& config, const T & instance);

		void add_instance(size_t slot_index, T&& instance);
		void add_instance(size_t slot_index, const T & instance);

	private:

        void new_slot(const RenderConfig& config, T&& first_instance);
        void new_slot(const RenderConfig& config, const T& first_instance);

	private:

		std::vector<RenderSlot<T>> m_slots;
		std::unordered_map<RenderConfig, size_t> m_slot_map;

	};


	template<typename T>
	inline RenderQueue<T>::RenderQueue()
	{
	}

	template<typename T>
	inline void RenderQueue<T>::submit(const RenderConfig& config, T&& instance)
	{
		const auto ix = m_slot_map.find(config);
		if (ix != m_slot_map.end())
		{
            add_instance(index, ix.second);
		}
        else
        {
			new_slot(config, instance);
        }
	}

	template<typename T>
	inline void RenderQueue<T>::submit(const RenderConfig& config, const T& instance)
	{
		const auto ix = m_slot_map.find(config);
		if (ix != m_slot_map.end())
		{
			add_instance(index, ix.second);
		}
		else
		{
			new_slot(config, instance);
		}
	}

	template<typename T>
	inline void RenderQueue<T>::add_instance(size_t slot_index, T&& instance)
	{
		m_slots[instance].add_instance(instance);
	}
	template<typename T>
	inline void RenderQueue<T>::add_instance(size_t slot_index, const T& instance)
	{
		m_slots[instance].add_instance(instance);
	}

    template<typename T>
    inline void RenderQueue<T>::new_slot(const RenderConfig& config, T&& first_instance)
    {
		const auto index = m_slots.size();
		m_slots.push_back(T(config, first_instance));

		m_slot_map[config] = index;
    }
	template<typename T>
	inline void RenderQueue<T>::new_slot(const RenderConfig& config, const T& first_instance)
	{
		const auto index = m_slots.size();
		m_slots.push_back(T(config, first_instance));

		m_slot_map[config] = index;
	}
}
