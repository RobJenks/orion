#include <bx/uint32_t.h>
#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"
#include "../engine/renderer/core/renderer_input_state.h"
#include "../container/container.h"
#include "../engine/renderer/geometry/geometry_manager.h"
#include "../engine/renderer/shader/shader_manager.h"
#include "../engine/renderer/texture/texture_manager.h"
#include "../util/log.h"

#include "orion.h"

namespace Orion
{
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
			LOG_ERROR("Fatal error initialising renderer (" << rendererInit << "), cannot continue");
			exit(1);
		}

		// Temporary
		Container tmp;
    }

    int Orion::shutdown()
    {
		// Shutdown primary components
		m_renderer.shutdown();

        return 0;
    }

    bool Orion::update()
    {
        if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
		{
			RendererInputState renderState;
			renderState.width = m_width;
			renderState.height = m_height;
			renderState.frame_ms = getFrameMs();
			renderState.mouse_state = &m_mouseState;
			renderState.view_at = { 0.0f, 0.0f, 0.0f };
			renderState.view_dir = { 0.0f, 0.0f, -35.0f };


			m_renderer.frame(renderState);
			_renderTemporaryScene();

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


	float Orion::getFrameMs() const
	{
		return float(bgfx::getStats()->cpuTimeFrame) * (1000.0f / float(bgfx::getStats()->cpuTimerFreq)) * 0.001f;
	}

	void Orion::_renderTemporaryScene()
	{
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

			const auto mesh = m_renderer.getGeometryManager().getMesh("cube");
			bgfx::setVertexBuffer(0, mesh.vertex_buffer);
			bgfx::setIndexBuffer(mesh.index_buffer);
			bgfx::setState(state);
		
			bgfx::submit(0, m_renderer.getShaderManager().getProgram("colour"));
		}

		const auto shader = m_renderer.getShaderManager().getProgram("inst_textured");
		const auto mesh = m_renderer.getGeometryManager().getMesh("quad");
		const auto uniform = m_renderer.getShaderManager().getUniform("s_texColor");
		const auto texture = m_renderer.getTextureManager().getTexture("fieldstone");
		uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;
		RenderConfig config(shader, mesh.vertex_buffer, mesh.index_buffer, state, { TextureUniformBinding(texture, uniform), TextureUniformBinding(), TextureUniformBinding(), TextureUniformBinding() });

		InstanceData inst;
		float scale[16], trans[16];
		bx::mtxScale(scale, 10.0f);
		for (int i = 0; i < 4; ++i)
		{
			bx::mtxTranslate(trans, -30.0f + (float(i) * 20.0f), 15.0f, 0.0f);
			bx::mtxMul(inst.transform, scale, trans);

			m_renderer.queue().primary().submit(config, inst);
		}

		/*{
			uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;
			uint32_t numInstances = 4;
			uint16_t instanceStride = sizeof(TexInstanceData);

			if (numInstances == bgfx::getAvailInstanceDataBuffer(numInstances, instanceStride))
			{
				bgfx::InstanceDataBuffer instances;
				bgfx::allocInstanceDataBuffer(&instances, numInstances, instanceStride);

				float scale[16], trans[16];
				bx::mtxScale(scale, 10.0f);
				TexInstanceData* data = (TexInstanceData*)instances.data;
				for (int i = 0; i < 4; ++i)
				{
					float* world = (float*)data->transform;
					bx::mtxTranslate(trans, -30.0f + (float(i) * 20.0f), 15.0f, 0.0f);
					bx::mtxMul(world, scale, trans);

					data += 1;
				}

				const auto mesh = m_renderer.getGeometryManager().getMesh("quad");
				bgfx::setVertexBuffer(0, mesh.vertex_buffer);
				bgfx::setIndexBuffer(mesh.index_buffer);
				bgfx::setInstanceDataBuffer(&instances);
				bgfx::setTexture(0,
					m_renderer.getShaderManager().getUniform("s_texColor"),
					m_renderer.getTextureManager().getTexture("fieldstone"));
				bgfx::setState(state);

				bgfx::submit(0, m_renderer.getShaderManager().getProgram("inst_textured"));
			}
		}*/
	}
}

ENTRY_IMPLEMENT_MAIN(
    Orion::Orion
    , "orion"
    , "orion"
    , ""
);
