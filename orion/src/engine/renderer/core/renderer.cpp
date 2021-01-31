#include "renderer.h"
#include "bgfx_utils.h"
#include "../../../util/log.h"
#include "renderer_input_state.h"

namespace Orion
{
	Renderer::Renderer()
		:
		m_shaders(),
		m_geometry(),
		m_textures(),
		m_gui(),
		m_camera(),
		m_queues(),
		m_renderStats()
	{	
	}

	ResultCode Renderer::initialise(uint32_t width, uint32_t height, uint32_t debug, bool runtime_debug, uint32_t reset, Args args)
	{
		LOG_INFO("Initialising renderer");

		// Initialise core runtime
		bgfx::Init init;
		init.type = bgfx::RendererType::Enum::OpenGL; // args.m_type;  // *** NOTE: D3D currently failing with shader creation error ***
		init.debug = runtime_debug;
		init.vendorId = args.m_pciId;
		init.resolution.width = width;
		init.resolution.height = height;
		init.resolution.reset = reset;

		LOG_INFO("Renderer configuration [type: " << init.type << ", pciId: " << init.vendorId << ", res: " << Vec2<uint32_t>(width, height) << "]");
		if (!bgfx::init(init))
		{
			RETURN_LOG_ERROR("Could not initialise core rendering libraries", ResultCodes::CouldNotInitEngineLibrary);
		}

		bgfx::reset(width, height);

		// Set debug levels
		bgfx::setDebug(debug);

		// Set view 0 clear state.
		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
		);

		// Initialise renderer components
		RETURN_ON_ERROR(initialiseShaderManager());
		RETURN_ON_ERROR(initialiseGeometryManger());
		RETURN_ON_ERROR(initialiseTextureManager());
		RETURN_ON_ERROR(initialiseGuiManger());
		RETURN_ON_ERROR(initialiseCamera());
		RETURN_ON_ERROR(initialiseRenderQueues());
		RETURN_ON_ERROR(initialiseRenderStats());

		return ResultCodes::Success;
	}


	ResultCode Renderer::initialiseShaderManager()
	{
		return m_shaders.initialise();
	}

	ResultCode Renderer::initialiseGeometryManger()
	{
		return m_geometry.initialise();
	}

	ResultCode Renderer::initialiseTextureManager()
	{
		return m_textures.initialise();
	}

	ResultCode Renderer::initialiseGuiManger()
	{
		return m_gui.initialise();
	}

	ResultCode Renderer::initialiseCamera()
	{
		return m_camera.initialise();
	}

	ResultCode Renderer::initialiseRenderQueues()
	{
		return m_queues.initialise();
	}

    ResultCode Renderer::initialiseRenderStats()
    {
        return m_renderStats.initialise();
    }

	ResultCode Renderer::frame(const RendererInputState& state)
	{
		// Pre-frame initialisation for all renderer components
		RETURN_ON_ERROR(beginFrame(state));

		// Render execution (including rq submission) for all renderer components
		RETURN_ON_ERROR(executeFrame(state));

		// Process the render queue and execute all rendering
		RETURN_ON_ERROR(render(state));

		// Post-frame cleanup for all renderer components
		RETURN_ON_ERROR(endFrame(state));

		return ResultCodes::Success;
	}
	 
	ResultCode Renderer::beginFrame(const RendererInputState& state)
	{
		RETURN_ON_ERROR(m_shaders.beginFrame(state));
		RETURN_ON_ERROR(m_geometry.beginFrame(state));
		RETURN_ON_ERROR(m_textures.beginFrame(state));
		RETURN_ON_ERROR(m_gui.beginFrame(state));
		RETURN_ON_ERROR(m_camera.beginFrame(state));
		RETURN_ON_ERROR(m_renderStats.beginFrame(state));

		// Ensures at least one draw call is submitted and backbuffer is therefore cleared between frames
		bgfx::touch(0);

		return ResultCodes::Success;
	}

	ResultCode Renderer::executeFrame(const RendererInputState& state)
	{
		RETURN_ON_ERROR(m_shaders.executeFrame(state));
		RETURN_ON_ERROR(m_geometry.executeFrame(state));
		RETURN_ON_ERROR(m_textures.executeFrame(state));
		RETURN_ON_ERROR(m_gui.executeFrame(state));
		RETURN_ON_ERROR(m_camera.executeFrame(state));
		RETURN_ON_ERROR(m_renderStats.executeFrame(state));

        // Render debug overlay
        renderDebugInfo();

		// Lock buffers and advance to next frame
		bgfx::frame();

		return ResultCodes::Success;
	}

	ResultCode Renderer::endFrame(const RendererInputState& state)
	{
		RETURN_ON_ERROR(m_shaders.endFrame(state));
		RETURN_ON_ERROR(m_geometry.endFrame(state));
		RETURN_ON_ERROR(m_textures.endFrame(state));
		RETURN_ON_ERROR(m_gui.endFrame(state));
		RETURN_ON_ERROR(m_camera.endFrame(state));
		RETURN_ON_ERROR(m_renderStats.endFrame(state));

		return ResultCodes::Success;
	}

	ResultCode Renderer::render(const RendererInputState& state)
	{
        // Process all renderer queues and submit geometry
        RETURN_ON_ERROR(processRenderQueues(state));

        // Reset and clean up render queues
        RETURN_ON_ERROR(resetRenderQueues());

        return ResultCodes::Success;
	}

	ResultCode Renderer::processRenderQueues(const RendererInputState& state)
	{
        // Process each render queue
        RETURN_ON_ERROR(processRenderQueue(m_queues.primary(), state));

        return ResultCodes::Success;
	}

	ResultCode Renderer::resetRenderQueues()
	{
		// Proces each render queue
        RETURN_ON_ERROR(resetRenderQueue(m_queues.primary()));

        return ResultCodes::Success;
	}

	void Renderer::submitWithRenderConfig(const RenderConfig& config)
	{
		bgfx::setVertexBuffer(0, config.get_vertex_buffer());
		bgfx::setIndexBuffer(config.get_index_buffer());
		bgfx::setState(config.get_state());

		const auto textures = config.get_textures().data();
		for (uint8_t i = 0, texture_count = config.get_textures().get_texture_count(); i < texture_count; ++i)
		{
			bgfx::setTexture(0, textures[i].uniform, textures[i].texture);
		}

		bgfx::submit(0, config.get_shader());
	}

    void Renderer::renderDebugInfo()
    {
        bgfx::dbgTextPrintf(0, 0, 0x0f, "FPS: %.1f", m_renderStats.getFps());
    }

	void Renderer::shutdown()
	{
		LOG_INFO("Shutting down renderer");

		shutdownShaderManager();
		shutdownGeometryManger();
		shutdownTextureManager();
		shutdownGuiManger();
		shutdownCamera();
		shutdownRenderQueues();
        shutdownRenderStats();

		LOG_INFO("Shutting down core render libraries");
		bgfx::shutdown();
	}

	void Renderer::submitImmediate(const RenderConfig& config)
	{
		submitWithRenderConfig(config);
	}

	void Renderer::submitImmediate(const RenderConfig& config, float transform[16])
	{
		bgfx::setTransform(transform);
		submitImmediate(config);
	}


    void Renderer::shutdownShaderManager()
	{
		m_shaders.shutdown();
	}

	void Renderer::shutdownGeometryManger()
	{
		m_geometry.shutdown();
	}

	void Renderer::shutdownTextureManager()
	{
		m_textures.shutdown();
	}

	void Renderer::shutdownGuiManger()
	{
		m_gui.shutdown();
	}

	void Renderer::shutdownCamera()
	{
		m_camera.shutdown();
	}

	void Renderer::shutdownRenderQueues()
	{
		m_queues.shutdown();
	}

    void Renderer::shutdownRenderStats()
    {
        m_renderStats.shutdown();
    }
}
