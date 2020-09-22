#include "imgui/imgui.h"
#include "../../../util/log.h"

#include "gui_manager.h"

namespace Orion
{
	GuiManager::GuiManager()
	{
	}

	ResultCode GuiManager::initialise()
	{
		LOG_INFO("Initialising GUI manager");

		imguiCreate();

		return ResultCodes::Success;
	}

	void GuiManager::shutdown()
	{
		LOG_INFO("Shutting down GUI manager");

		imguiDestroy();
	}
}
