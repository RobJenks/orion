#pragma once

#include <bgfx_utils.h>
#include "../../../util/bgfx_support.h"

namespace Orion
{
	struct TextureUniformBinding
	{
		const bgfx::TextureHandle texture;
		const bgfx::UniformHandle uniform;

		inline constexpr TextureUniformBinding()
			:
			TextureUniformBinding({ BgfxSupport::INVALID_HANDLE }, { BgfxSupport::INVALID_HANDLE })
		{
		}

		inline constexpr TextureUniformBinding(bgfx::TextureHandle textureHandle, bgfx::UniformHandle uniformHandle)
			:
			texture(textureHandle),
			uniform(uniformHandle)
		{
		}

		inline constexpr bool operator==(const TextureUniformBinding& other) const
		{
			return ( texture.idx == other.texture.idx &&
					 uniform.idx == other.uniform.idx);
		}

		inline constexpr bool operator!=(const TextureUniformBinding& other) const
		{
			return !(*this == other);
		}
	};
}
