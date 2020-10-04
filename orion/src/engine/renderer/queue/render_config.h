#pragma once

#include <functional>
#include "bgfx_utils.h"

namespace Orion
{
	struct RenderConfig
	{
	public:

		RenderConfig(bgfx::ProgramHandle shader);

		inline const bgfx::ProgramHandle get_shader() const { return m_shader; }

		inline const size_t hash() const { return m_hash; }

	private:

		size_t calculate_hash() const;
		

	private:
		const bgfx::ProgramHandle m_shader;

		const size_t m_hash;	// Calculated on construction, objects are immutable
		



	};
}

// Hash function for immutable render config structures
namespace std
{
	template <>
	struct hash<Orion::RenderConfig>
	{
		std::size_t operator()(const Orion::RenderConfig& rc) const
		{
			return rc.hash();
		}
	};

}
