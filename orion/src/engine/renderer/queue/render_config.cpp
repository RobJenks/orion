#include "render_config.h"

#define RENDER_CONFIG_FAST_HASH

#ifdef RENDER_CONFIG_FAST_HASH
#	define HASH_COMP(T,comp) comp
#else
#	define HASH_COMP(T,comp) std::hash<T>()(comp)
#endif

namespace Orion
{
	RenderConfig::RenderConfig(bgfx::ProgramHandle shader)
		:
		m_shader(shader),
		m_hash(calculate_hash())
	{
	}

	size_t RenderConfig::calculate_hash() const
	{
		size_t res = 17;
		res = res * 31 + HASH_COMP(uint16_t, m_shader.idx);

		return res;
	}
}

#undef HASH_COMP
