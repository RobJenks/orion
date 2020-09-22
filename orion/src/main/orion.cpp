#include <bx/uint32_t.h>
#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"
#include "../container/container.h"



#include "../engine/renderer/geometry/vertex_definitions.h"
#include "../util/log.h"


#include "orion.h"


namespace Orion
{
    // Temporary
    static VertexDefinitions::PosColorVertex s_cubeVertices[] =
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
	struct InstanceData
	{
		float transform[16];
		float colour[4];
	};
	struct TexInstanceData
	{
		float transform[16];
	};
    bgfx::VertexBufferHandle m_vb;
    bgfx::IndexBufferHandle m_ib;
    bgfx::VertexBufferHandle m_qvb;
    bgfx::IndexBufferHandle m_qib;
	bgfx::UniformHandle s_texColor;
    bgfx::ProgramHandle m_program;
    bgfx::ProgramHandle m_inst_program;
	bgfx::TextureHandle m_textureColor;

	static VertexDefinitions::PosTexVertex s_quadVertices[] =
	{
		{-1.0f,  1.0f,  1.0f, 0.0f, 1.0f },
		{ 1.0f,  1.0f,  1.0f, 1.0f, 1.0f },
		{-1.0f, -1.0f,  1.0f, 0.0f, 0.0f },
		{ 1.0f, -1.0f,  1.0f, 1.0f, 0.0f }
	};
	static const uint16_t s_quadTriList[] =
	{
		2, 1, 0, // 0, 1, 2,
		3, 1, 2  // 2, 1, 3
	};



    Orion::Orion(const char* name, const char* description, const char* url)
        :
        entry::AppI(name, description, url),
        m_width(0U),
        m_height(0U),
        m_debug(0U),
        m_reset(0U),
		m_renderer()
    {
    }

    void Orion::init(int32_t argc, const char* const* argv, uint32_t width, uint32_t height)
    {
		// Temporary
		Log::initialise("orion.log");

        Args args(argc, argv);

		const uint32_t debug = BGFX_DEBUG_TEXT;
		const uint32_t reset = BGFX_RESET_VSYNC;

		const auto rendererInit = m_renderer.initialise(width, height, debug, RENDERER_RUNTIME_DEBUG_ENABLED, reset, args);
		if (ResultCodes::isError(rendererInit))
		{
			exit(1);	// *** HANDLE ***
		}

		// Temporary
		Container tmp;
		//exit(tmp.tmp());


        m_vb = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), VertexDefinitions::PosColorVertex::ms_layout);
        m_ib = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

		m_qvb = bgfx::createVertexBuffer(bgfx::makeRef(s_quadVertices, sizeof(s_quadVertices)), VertexDefinitions::PosTexVertex::ms_layout);
		m_qib = bgfx::createIndexBuffer(bgfx::makeRef(s_quadTriList, sizeof(s_quadTriList)));

		s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

		m_textureColor = loadTexture("textures/fieldstone-rgba.dds");

        m_program = loadProgram("vs_cubes", "fs_cubes");
		m_inst_program = loadProgram("vs_instanced_texture", "fs_instanced_texture");
    }

    int Orion::shutdown()
    {
        // Cleanup GUI
        imguiDestroy();

        // Temporary
        bgfx::destroy(m_vb);
        bgfx::destroy(m_qvb);
		bgfx::destroy(m_ib);
		bgfx::destroy(m_qib);
		bgfx::destroy(m_textureColor);
		bgfx::destroy(s_texColor);
        bgfx::destroy(m_program);
        bgfx::destroy(m_inst_program);

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
			static float timeRot = 0.0f;
			timeRot += float(bgfx::getStats()->cpuTimeFrame) * (1000.0f / float(bgfx::getStats()->cpuTimerFreq)) * 0.001f;
           {
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
			{
				uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;
				uint32_t numInstances = 4;
				uint16_t instanceStride = sizeof(TexInstanceData);

				if (numInstances == bgfx::getAvailInstanceDataBuffer(numInstances, instanceStride))
				{
					bgfx::InstanceDataBuffer instances;
					bgfx::allocInstanceDataBuffer(&instances, numInstances, instanceStride);

					float scale[16], trans[16];
					bx::mtxScale(scale, 10.0f);
					TexInstanceData *data = (TexInstanceData*)instances.data;
					for (int i = 0; i < 4; ++i)
					{
						float* world = (float*)data->transform;
						bx::mtxTranslate(trans, -30.0f + (float(i) * 20.0f), 15.0f, 0.0f);
						bx::mtxMul(world, scale, trans);

						data += 1;
					}

					bgfx::setVertexBuffer(0, m_qvb);
					bgfx::setIndexBuffer(m_qib);
					bgfx::setInstanceDataBuffer(&instances);
					bgfx::setTexture(0, s_texColor, m_textureColor);
					bgfx::setState(state);

					bgfx::submit(0, m_inst_program);
				}
			}

            // Debug print FPS
            bgfx::dbgTextClear();

			//m_renderStats.frame(bgfx::getStats());
            //bgfx::dbgTextPrintf(0, 0, 0x0f, "FPS: %.1f", m_renderStats.getFps());

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
