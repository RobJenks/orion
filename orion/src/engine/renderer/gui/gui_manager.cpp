#include "imgui/imgui.h"
#include "../../../util/log.h"
#include "../core/renderer_input_state.h"

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

	ResultCode GuiManager::beginFrame(const RendererInputState& state)
	{
		const entry::MouseState& mouse_state = *state.mouse_state;
		imguiBeginFrame(
			   mouse_state.m_mx
			,  mouse_state.m_my
			, (mouse_state.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
			| (mouse_state.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
			| (mouse_state.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
			,  mouse_state.m_mz
			, uint16_t(state.width)
			, uint16_t(state.height)
		);

		return ResultCodes::Success;
	}

	ResultCode GuiManager::executeFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	ResultCode GuiManager::endFrame(const RendererInputState& state)
	{
		(void)state;

		imguiEndFrame();

		return ResultCodes::Success;
	}

	void GuiManager::shutdown()
	{
		LOG_INFO("Shutting down GUI manager");

		imguiDestroy();
	}
}
