#include "imgui/imgui.h"

#include "gui_manager.h"

namespace Orion
{
	GuiManager::GuiManager()
	{
	}

	ResultCode GuiManager::initialise()
	{
		imguiCreate();

		return ResultCodes::Success;
	}

	void GuiManager::shutdown()
	{
		imguiDestroy();
	}
}
