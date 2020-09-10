#include <bx/uint32_t.h>
#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"

#include "orion.h"


namespace Orion
{
	Orion::Orion(const char* _name, const char* _description, const char* _url)
		:
		entry::AppI(_name, _description, _url),
		m_debug(0U), m_width(0U), m_height(0U), m_reset(0U)
	{
	}

	void Orion::init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height)
	{
		Args args(_argc, _argv);

		m_width = _width;
		m_height = _height;
		m_debug = BGFX_DEBUG_TEXT;
		m_reset = BGFX_RESET_VSYNC;

		bgfx::Init init;
		init.type = args.m_type;
		init.vendorId = args.m_pciId;
		init.resolution.width = m_width;
		init.resolution.height = m_height;
		init.resolution.reset = m_reset;
		bgfx::init(init);

		// Enable debug text.
		bgfx::setDebug(m_debug);

		// Set view 0 clear state.
		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
		);

		imguiCreate();
	}

	int Orion::shutdown()
	{
		imguiDestroy();

		// Shutdown bgfx.
		bgfx::shutdown();

		return 0;
	}

	bool Orion::update()
	{
		if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
		{
			imguiBeginFrame(m_mouseState.m_mx
				, m_mouseState.m_my
				, (m_mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
				| (m_mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
				| (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
				, m_mouseState.m_mz
				, uint16_t(m_width)
				, uint16_t(m_height)
			);

			// (UI operations)

			imguiEndFrame();

			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			// Debug print FPS
			bgfx::dbgTextClear();

			const auto stats = bgfx::getStats();
			const double toMsCpu = 1000.0 / stats->cpuTimerFreq;
			const double frameMs = double(stats->cpuTimeFrame) * toMsCpu;
			bgfx::dbgTextPrintf(0, 1, 0x0f, "FPS: %.1f", 1000.0f / frameMs);

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();

			return true;
		}

		return false;
	}
}

ENTRY_IMPLEMENT_MAIN(
	Orion::Orion
	, "orion"
	, "orion"
	, ""
);
