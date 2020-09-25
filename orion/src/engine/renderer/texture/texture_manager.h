#pragma once

#include <unordered_map>
#include "../../../util/result_code.h"
struct RendererInputState;

namespace Orion
{
	class TextureManager
	{
	public:

		TextureManager();

		ResultCode initialise();

		ResultCode beginFrame(const RendererInputState& state);
		ResultCode executeFrame(const RendererInputState& state);
		ResultCode endFrame(const RendererInputState& state);

		inline bgfx::TextureHandle getTexture(const std::string& name) const { return m_textures.at(name); }

		void shutdown();

	private:

		ResultCode initialiseTextures();

		ResultCode loadTextureResource(const std::string& name, const std::string& path);

		void shutdownTextureResources();

	private:

		std::unordered_map<std::string, bgfx::TextureHandle> m_textures;

	};
}
