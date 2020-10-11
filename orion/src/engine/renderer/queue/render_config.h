#pragma once

#include <array>
#include <functional>
#include "bgfx_utils.h"
#include "../shader/uniform_binding.h"

namespace Orion
{
	struct RenderConfig
	{
	public:
		static const size_t CONFIG_TEXTURE_COUNT = 4U;

		RenderConfig(bgfx::ProgramHandle shader, bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer,
					 uint64_t state, std::array<TextureUniformBinding, CONFIG_TEXTURE_COUNT> textures);

		inline const bgfx::ProgramHandle get_shader() const { return m_shader; }
		inline const bgfx::VertexBufferHandle get_vertex_buffer() const { return m_vb; }
		inline const bgfx::IndexBufferHandle get_index_buffer() const { return m_ib; }
		inline const uint64_t get_state() const { return m_state; }

		inline const std::array<TextureUniformBinding, CONFIG_TEXTURE_COUNT> & get_textures() const { return m_textures; }
		inline const TextureUniformBinding get_texture(size_t index) const { return m_textures[index]; }

		inline const uint8_t get_texture_count() const { return m_texture_count; }
		inline const size_t hash() const { return m_hash; }

		bool operator==(const RenderConfig& other) const;

	private:

		uint8_t determine_texture_count(const std::array<TextureUniformBinding, CONFIG_TEXTURE_COUNT>& textures) const;
		size_t calculate_hash() const;
		

	private:
		const bgfx::ProgramHandle m_shader;
		const bgfx::VertexBufferHandle m_vb;
		const bgfx::IndexBufferHandle m_ib;
		const uint64_t m_state;
		const std::array<TextureUniformBinding, CONFIG_TEXTURE_COUNT> m_textures;

		// Calculated fields
		const uint8_t m_texture_count;	// Number of texture slots being used
		const size_t m_hash;			// Calculated on construction, objects are immutable
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
