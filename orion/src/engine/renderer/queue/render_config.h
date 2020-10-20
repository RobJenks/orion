#pragma once

#include <array>
#include <functional>
#include "bgfx_utils.h"
#include "../shader/uniform_binding.h"

#define RENDER_CONFIG_FAST_HASH

#ifdef RENDER_CONFIG_FAST_HASH
#	define HASH_COMP(T,comp) comp
#else
#	define HASH_COMP(T,comp) std::hash<T>()(comp)
#endif

namespace Orion
{
	struct RenderConfig
	{
	public:
		struct Textures
		{
			static constexpr size_t MAX_TEXTURES = 4U;

			static const Textures NO_TEXTURES;
			//constexpr static inline Textures none() { return NO_TEXTURES; }
			constexpr Textures(const std::array<TextureUniformBinding, MAX_TEXTURES>& textures);
			constexpr Textures(std::array<TextureUniformBinding, MAX_TEXTURES>&& textures);
			constexpr Textures(const TextureUniformBinding & single_texture);
			constexpr Textures(TextureUniformBinding && single_texture);

			constexpr inline const std::array<TextureUniformBinding, MAX_TEXTURES>& data() const { return m_textures; }
			constexpr inline const TextureUniformBinding get_texture(size_t index) const { return m_textures[index]; }
			constexpr inline const uint8_t get_texture_count() const { return m_texture_count; }

			constexpr bool operator==(const Textures& other) const;
			constexpr size_t calculate_hash_component() const;

		private:
			
			constexpr uint8_t determine_texture_count() const;

			const std::array<TextureUniformBinding, MAX_TEXTURES> m_textures;
			const uint8_t m_texture_count;	// Calculated field; number of texture slots being used
		};

		constexpr RenderConfig(bgfx::ProgramHandle shader, bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer,
							   uint64_t state, const Textures & textures);

		constexpr RenderConfig(bgfx::ProgramHandle shader, bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer,
							   uint64_t state, Textures && textures);

		constexpr RenderConfig(bgfx::ProgramHandle shader, bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer,
							   uint64_t state);

		constexpr inline const bgfx::ProgramHandle get_shader() const { return m_shader; }
		constexpr inline const bgfx::VertexBufferHandle get_vertex_buffer() const { return m_vb; }
		constexpr inline const bgfx::IndexBufferHandle get_index_buffer() const { return m_ib; }
		constexpr inline const uint64_t get_state() const { return m_state; }
		constexpr inline const Textures & get_textures() const { return m_textures; }
		
		constexpr inline const size_t hash() const { return m_hash; }

		constexpr bool operator==(const RenderConfig& other) const;

	private:

		constexpr size_t calculate_hash() const;
		

	private:
		const bgfx::ProgramHandle m_shader;
		const bgfx::VertexBufferHandle m_vb;
		const bgfx::IndexBufferHandle m_ib;
		const uint64_t m_state;
		const Textures m_textures;

		// Calculated fields
		const size_t m_hash;			// Calculated on construction, objects are immutable
	};
}

// Definitions
namespace Orion
{
	constexpr RenderConfig::RenderConfig(bgfx::ProgramHandle shader, bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer,
		uint64_t state, const Textures& textures)
		:
		m_shader(shader),
		m_vb(vertex_buffer),
		m_ib(index_buffer),
		m_state(state),
		m_textures(textures),

		m_hash(calculate_hash())
	{
	}

	constexpr RenderConfig::RenderConfig(bgfx::ProgramHandle shader, bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer, uint64_t state, Textures&& textures)
		:
		m_shader(shader),
		m_vb(vertex_buffer),
		m_ib(index_buffer),
		m_state(state),
		m_textures(textures),

		m_hash(calculate_hash())
	{
	}

	constexpr RenderConfig::RenderConfig(bgfx::ProgramHandle shader, bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer, uint64_t state)
		:
		RenderConfig(shader, vertex_buffer, index_buffer, state, Textures::NO_TEXTURES)
	{
	}

	constexpr size_t RenderConfig::calculate_hash() const
	{
		size_t res = 17;
		res = res * 31 + HASH_COMP(uint16_t, m_shader.idx);
		res = res * 31 + HASH_COMP(uint16_t, m_vb.idx);
		res = res * 31 + HASH_COMP(uint16_t, m_ib.idx);
		res = res * 31 + HASH_COMP(uint64_t, m_state);
		res = res * 31 + m_textures.calculate_hash_component();

		return res;
	}

	constexpr bool RenderConfig::operator==(const RenderConfig& other) const
	{
		// TODO: Can delegate this to the hash function, assuming all idx combinations are unique?
		return
			m_shader.idx == other.m_shader.idx &&
			m_vb.idx == other.m_vb.idx &&
			m_ib.idx == other.m_ib.idx &&
			m_state == other.m_state &&
			m_textures == other.m_textures;
	}

	constexpr RenderConfig::Textures::Textures(const std::array<TextureUniformBinding, MAX_TEXTURES>& textures)
		:
		m_textures(textures),
		m_texture_count(determine_texture_count())
	{
	}

	constexpr RenderConfig::Textures::Textures(std::array<TextureUniformBinding, MAX_TEXTURES>&& textures)
		:
		m_textures(textures),
		m_texture_count(determine_texture_count())
	{
	}

	constexpr RenderConfig::Textures::Textures(const TextureUniformBinding& single_texture)
		:
		Textures({ single_texture, TextureUniformBinding(), TextureUniformBinding(), TextureUniformBinding() })
	{
	}

	constexpr RenderConfig::Textures::Textures(TextureUniformBinding&& single_texture)
		:
		Textures({ single_texture, TextureUniformBinding(), TextureUniformBinding(), TextureUniformBinding() })
	{
	}

	constexpr bool RenderConfig::Textures::operator==(const Textures& other) const
	{
		for (size_t i = 0; i < MAX_TEXTURES; ++i)
		{
			if (m_textures[i] != other.m_textures[i]) return false;
		}

		return true;
	}

	// Textures must be bound in-order; texture binding will stop at the first invalid texture handle
	constexpr uint8_t RenderConfig::Textures::determine_texture_count() const
	{
		uint8_t count = 0U;
		while (BgfxSupport::isValid(m_textures[count].texture) && BgfxSupport::isValid(m_textures[count].uniform)) { ++count; }

		return count;
	}

	// Calculates the texture component of an overall hash; does not therefore include the initial hash prime
	constexpr size_t RenderConfig::Textures::calculate_hash_component() const
	{
		size_t res = 0U;
		for (size_t i = 0; i < MAX_TEXTURES; ++i)
		{
			res = res * 31 + HASH_COMP(uint16_t, m_textures[i].texture.idx);
			res = res * 31 + HASH_COMP(uint16_t, m_textures[i].uniform.idx);
		}

		return res;
	}
}

// Hash function for immutable render config structures
namespace std
{
	template <>
	struct hash<Orion::RenderConfig>
	{
		size_t operator()(const Orion::RenderConfig& rc) const
		{
			return rc.hash();
		}
	};

}


#undef HASH_COMP
