#include <bx/uint32_t.h>
#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"

#include "orion.h"


namespace Orion
{
	// Temporary
	bgfx::VertexLayout PosColorVertex::ms_layout;
	static PosColorVertex s_cubeVertices[] =
	{
		{-1.0f,  1.0f,  1.0f, 0xff000000 },
		{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
		{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
		{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
		{-1.0f,  1.0f, -1.0f, 0xffff0000 },
		{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
		{-1.0f, -1.0f, -1.0f, 0xffffff00 },
		{ 1.0f, -1.0f, -1.0f, 0xffffffff },
	};
	static const uint16_t s_cubeTriList[] =
	{
		0, 1, 2, // 0
		1, 3, 2,
		4, 6, 5, // 2
		5, 6, 7,
		0, 2, 4, // 4
		4, 2, 6,
		1, 5, 3, // 6
		5, 7, 3,
		0, 4, 1, // 8
		4, 5, 1,
		2, 3, 6, // 10
		6, 3, 7,
	};
	bgfx::VertexBufferHandle m_vb;
	bgfx::IndexBufferHandle m_ib;
	bgfx::ProgramHandle m_program;




	Orion::Orion(const char* _name, const char* _description, const char* _url)
		:
		entry::AppI(_name, _description, _url),
		m_width(0U),
		m_height(0U),
		m_debug(0U),
		m_reset(0U)
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

		// Initialise GUI
		imguiCreate();

		// Temporary
		PosColorVertex::init();
		m_vb = bgfx::createVertexBuffer(
			bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
			PosColorVertex::ms_layout
		);
		m_ib = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
		m_program = loadProgram("vs_cubes", "fs_cubes");
	}

	int Orion::shutdown()
	{
		// Cleanup GUI
		imguiDestroy();

		// Temporary
		bgfx::destroy(m_vb);
		bgfx::destroy(m_program);

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

			// Temporary (probably): Set view and projection matrix for view 0.
			const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
			const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };
			{
				float view[16];
				bx::mtxLookAt(view, eye, at);

				float proj[16];
				bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
				bgfx::setViewTransform(0, view, proj);

				// Set view 0 default viewport.
				bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
			}

			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			// Temporary
			{
				static float timeRot = 0.0f;
				timeRot += float(bgfx::getStats()->cpuTimeFrame) * (1000.0f / float(bgfx::getStats()->cpuTimerFreq)) * 0.001f;

				uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;

				float rot[16], scale[16], world[16];
				bx::mtxRotateXY(rot, 0.42f + timeRot, 0.74f + timeRot);
				bx::mtxScale(scale, 8.0f);
				bx::mtxMul(world, scale, rot);
				bgfx::setTransform(world);

				bgfx::setVertexBuffer(0, m_vb);
				bgfx::setIndexBuffer(m_ib);
				bgfx::setState(state);

				bgfx::submit(0, m_program);
			}


			// Debug print FPS
			bgfx::dbgTextClear();

			const auto stats = bgfx::getStats();
			const double toMsCpu = 1000.0 / stats->cpuTimerFreq;
			const double frameMs = double(stats->cpuTimeFrame) * toMsCpu;

			static const double fpsRenderInterval = 1000.0;
			static double timeUntilNextFpsRender = 0.0;
			static double cumulativeFrameTime = 0.0, currentFps = 0.0;
			static int samples = 0;

			cumulativeFrameTime += frameMs;
			samples += 1;

			if ((timeUntilNextFpsRender -= frameMs) <= 0.0)
			{
				timeUntilNextFpsRender += fpsRenderInterval;
				currentFps = 1000.0 / (cumulativeFrameTime / samples);	// Deal with div/0
			}
			bgfx::dbgTextPrintf(0, 1, 0x0f, "FPS: %.1f", currentFps);

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
