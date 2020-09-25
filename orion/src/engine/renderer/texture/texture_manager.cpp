#include <algorithm>
#include "../../../util/log.h"
#include "../core/renderer_input_state.h"
#include "bgfx_utils.h"

#include "texture_manager.h"

namespace Orion
{
	TextureManager::TextureManager()
	{
	}

	ResultCode TextureManager::initialise()
	{
		LOG_INFO("Initialising texture manager");

		RETURN_ON_ERROR(initialiseTextures());

		return ResultCodes::Success;
	}

	ResultCode TextureManager::initialiseTextures()
	{
		LOG_INFO("Initialising texture resources");

		// Temporary: textures will be loaded from configuration in future
		std::vector<std::tuple<std::string, std::string>> temporaryResources {
			{ "fieldstone", "textures/fieldstone-rgba.dds" }
		};

		ResultCode aggregate = ResultCodes::Success;
		for (auto resource : temporaryResources)
		{
			auto result = loadTextureResource(std::get<0>(resource), std::get<1>(resource));
			if (!ResultCodes::isSuccess(result) && ResultCodes::isSuccess(aggregate))
			{
				aggregate = result;
			}
		}

		if (!ResultCodes::isSuccess(aggregate))
		{
			LOG_WARN("At least one failure while loading texture resources (" << aggregate << ")");
		}

		return ResultCodes::Success;
	}

	ResultCode TextureManager::loadTextureResource(const std::string& name, const std::string& path)
	{
		if (m_textures.find(name) != m_textures.end())
		{
			RETURN_LOG_ERROR("Cannot load texture resource; texture \"" << name << "\" already exists", ResultCodes::CannotLoadDuplicateTexture);
		}

		auto handle = loadTexture(path.c_str());
		if (!bgfx::isValid(handle))
		{
			RETURN_LOG_ERROR("Failed to load texture resource \"" << name << "\"", ResultCodes::FailedToLoadTextureResource);
		}

		m_textures[name] = handle;
		return ResultCodes::Success;
	}


	ResultCode TextureManager::beginFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	ResultCode TextureManager::executeFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	ResultCode TextureManager::endFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	void TextureManager::shutdown()
	{
		LOG_INFO("Shutting down texture manager");

		shutdownTextureResources();
	}

	void TextureManager::shutdownTextureResources()
	{
		LOG_INFO("Releasing all texture resources");

		std::for_each(m_textures.begin(), m_textures.end(), [](auto& texture) { bgfx::destroy(texture.second); });
		m_textures.clear();
	}
}
