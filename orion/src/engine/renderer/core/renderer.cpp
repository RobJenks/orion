#include "renderer.h"
#include "bgfx_utils.h"
#include "../../../util/log.h"
#include "renderer_input_state.h"

namespace Orion
{
	Renderer::Renderer()
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
		if (!bgfx::init(init))
		{
			RETURN_LOG_ERROR("Could not initialise core rendering library", ResultCodes::CouldNotInitEngineLibrary);
		}

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

	ResultCode Renderer::frame(const RendererInputState& state)
	{
		RETURN_ON_ERROR(beginFrame(state));
		RETURN_ON_ERROR(executeFrame(state));
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

		return ResultCodes::Success;
	}

	ResultCode Renderer::endFrame(const RendererInputState& state)
	{
		RETURN_ON_ERROR(m_shaders.endFrame(state));
		RETURN_ON_ERROR(m_geometry.endFrame(state));
		RETURN_ON_ERROR(m_textures.endFrame(state));
		RETURN_ON_ERROR(m_gui.endFrame(state));
		RETURN_ON_ERROR(m_camera.endFrame(state));

		return ResultCodes::Success;
	}



	void Renderer::shutdown()
	{
		LOG_INFO("Shutting down renderer");

		shutdownShaderManager();
		shutdownGeometryManger();
		shutdownTextureManager();
		shutdownGuiManger();
		shutdownCamera();
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

}
