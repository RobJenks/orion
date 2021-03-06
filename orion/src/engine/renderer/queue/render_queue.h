#pragma once

#include <vector>
#include <unordered_map>
#include "../../../util/log.h"
#include "render_slot.h"
#include "render_config.h"

namespace Orion
{
	template <typename T>
	class RenderQueue
	{
	public:
		RenderQueue(const std::string& id);

		ResultCode initialise();

		void submit(const RenderConfig& config, T&& instance);
		void submit(const RenderConfig& config, const T & instance);

		void add_instance(size_t slot_index, T&& instance);
		void add_instance(size_t slot_index, const T & instance);

		inline const std::vector<RenderSlot<T>>& getSlots() const { return m_slots; }

		ResultCode reset();

		void shutdown();

	private:

        void new_slot(const RenderConfig& config, T&& first_instance);
        void new_slot(const RenderConfig& config, const T& first_instance);

	private:

		std::string m_id;

		std::vector<RenderSlot<T>> m_slots;
		std::unordered_map<RenderConfig, size_t, std::hash<RenderConfig>> m_slot_map;

	};


	template<typename T>
	inline RenderQueue<T>::RenderQueue(const std::string& id)
		:
		m_id(id)
	{
	}

	template<typename T>
	inline ResultCode RenderQueue<T>::initialise()
	{
		LOG_INFO("Initialising render queue \"" << m_id << "\"");

		return ResultCodes::Success;
	}

	template<typename T>
	inline void RenderQueue<T>::submit(const RenderConfig& config, T&& instance)
	{
		const auto ix = m_slot_map.find(config);
		if (ix != m_slot_map.end())
		{
            add_instance(ix->second, instance);
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
			add_instance(ix->second, instance);
		}
		else
		{
			new_slot(config, instance);
		}
	}

	template<typename T>
	inline void RenderQueue<T>::add_instance(size_t slot_index, T&& instance)
	{
		m_slots[slot_index].add_instance(instance);
	}
	template<typename T>
	inline void RenderQueue<T>::add_instance(size_t slot_index, const T& instance)
	{
		m_slots[slot_index].add_instance(instance);
	}

	template<typename T>
	inline ResultCode RenderQueue<T>::reset()
	{
		// TODO: For now, just clear all data between frames.  Later, persist those which are in use for a number of frames for efficiency
		m_slots.clear();
		m_slot_map.clear();
	}

	template<typename T>
	inline void RenderQueue<T>::shutdown()
	{
		LOG_INFO("Shutting down render queue \"" << m_id << "\"");
	}

    template<typename T>
    inline void RenderQueue<T>::new_slot(const RenderConfig& config, T&& first_instance)
    {
		const auto index = m_slots.size();
		m_slots.push_back(RenderSlot<T>(config, first_instance));

		m_slot_map[config] = index;
    }
	template<typename T>
	inline void RenderQueue<T>::new_slot(const RenderConfig& config, const T& first_instance)
	{
		const auto index = m_slots.size();
		m_slots.push_back(RenderSlot<T>(config, first_instance));

		m_slot_map[config] = index;
	}
}
