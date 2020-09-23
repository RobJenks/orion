#include "../../../util/log.h"
#include "../core/renderer_input_state.h"

#include "texture_manager.h"

namespace Orion
{
	TextureManager::TextureManager()
	{
	}

	ResultCode TextureManager::initialise()
	{
		LOG_INFO("Initialising shader manager");

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
		LOG_INFO("Shutting down shader manager");
	}
}
