#pragma once

#include <bgfx_utils.h>

namespace Orion
{
	struct TextureUniformBinding
	{
		bgfx::TextureHandle texture;
		bgfx::UniformHandle uniform;

		inline TextureUniformBinding()
			:
			TextureUniformBinding(BGFX_INVALID_HANDLE, BGFX_INVALID_HANDLE)
		{
		}

		inline TextureUniformBinding(bgfx::TextureHandle textureHandle, bgfx::UniformHandle uniformHandle)
			:
			texture(textureHandle),
			uniform(uniformHandle)
		{
		}

		inline bool operator==(const TextureUniformBinding& other) const
		{
			return ( texture.idx == other.texture.idx &&
					 uniform.idx == other.uniform.idx);
		}

		inline bool operator!=(const TextureUniformBinding& other) const
		{
			return !(*this == other);
		}
	};
}
