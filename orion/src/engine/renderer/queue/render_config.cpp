#include "render_config.h"

#define RENDER_CONFIG_FAST_HASH

#ifdef RENDER_CONFIG_FAST_HASH
#	define HASH_COMP(T,comp) comp
#else
#	define HASH_COMP(T,comp) std::hash<T>()(comp)
#endif

namespace Orion
{
	RenderConfig::RenderConfig(bgfx::ProgramHandle shader, bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer,
							   uint64_t state, std::array<bgfx::TextureHandle, CONFIG_TEXTURE_COUNT> textures)
		:
		m_shader(shader),
		m_vb(vertex_buffer),
		m_ib(index_buffer),
		m_state(state),
		m_textures(textures),

		m_texture_count(determine_texture_count(textures)),
		m_hash(calculate_hash())
	{
	}

	// Textures must be bound in-order; texture binding will stop at the first invalid texture handle
	uint8_t RenderConfig::determine_texture_count(const std::array<bgfx::TextureHandle, CONFIG_TEXTURE_COUNT>& textures) const
	{
		uint8_t count = 0U;
		while (bgfx::isValid(textures[count])) { ++count; }

		return count;
	}

	size_t RenderConfig::calculate_hash() const
	{
		size_t res = 17;
		res = res * 31 + HASH_COMP(uint16_t, m_shader.idx);
		res = res * 31 + HASH_COMP(uint16_t, m_vb.idx);
		res = res * 31 + HASH_COMP(uint16_t, m_ib.idx);
		res = res * 31 + HASH_COMP(uint64_t, m_state);

		// Assumes CONFIG_TEXTURE_COUNT == 4U
		res = res * 31 + HASH_COMP(uint16_t, m_textures[0].idx);
		res = res * 31 + HASH_COMP(uint16_t, m_textures[1].idx);
		res = res * 31 + HASH_COMP(uint16_t, m_textures[2].idx);
		res = res * 31 + HASH_COMP(uint16_t, m_textures[3].idx);
		
		return res;
	}

	bool RenderConfig::operator==(const RenderConfig& other) const
	{
		return
			m_shader.idx == other.m_shader.idx &&
			m_vb.idx == other.m_ib.idx &&
			m_ib.idx == other.m_ib.idx &&
			m_state == other.m_state &&

			// Assumes CONFIG_TEXTURE_COUNT == 4U
			m_textures[0].idx == other.m_textures[0].idx &&
			m_textures[1].idx == other.m_textures[1].idx &&
			m_textures[2].idx == other.m_textures[2].idx &&
			m_textures[3].idx == other.m_textures[3].idx;
	}
}

#undef HASH_COMP
