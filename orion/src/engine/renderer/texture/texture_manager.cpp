#include "../../../util/log.h"

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

	void TextureManager::shutdown()
	{
		LOG_INFO("Shutting down shader manager");
	}
}
