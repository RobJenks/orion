#pragma once

#include <stdint.h>
#include <array>
#include "../../../util/result_code.h"
#include "../../../math/vec2.h"
#include "../shader/shader_manager.h"
#include "../geometry/geometry_manager.h"
#include "../queue/render_queues.h"
#include "../queue/render_queue.h"
#include "../texture/texture_manager.h"
#include "../gui/gui_manager.h"
#include "../camera/camera.h"
#include "../debug/render_stats.h"
struct RendererInputState;
struct Args;

namespace Orion
{
	class Renderer
	{
	public:

		Renderer();

		ResultCode initialise(uint32_t width, uint32_t height, uint32_t debug, bool runtime_debug, uint32_t reset, Args args);

		ResultCode frame(const RendererInputState & state);

		inline ShaderManager& getShaderManager() { return m_shaders; }
		inline GeometryManager& getGeometryManager() { return m_geometry; }
		inline TextureManager& getTextureManager() { return m_textures; }
		inline GuiManager& getGuiManager() { return m_gui; }
		inline Camera& getCamera() { return m_camera; }
		inline RenderQueues& getQueues() { return m_queues; }

		const inline ShaderManager& getShaderManager() const { return m_shaders; }
		const inline GeometryManager& getGeometryManager() const { return m_geometry; }
		const inline TextureManager& getTextureManager() const { return m_textures; }
		const inline GuiManager& getGuiManager() const { return m_gui; }
		const inline Camera& getCamera() const { return m_camera; }
		const inline RenderQueues& getQueues() const { return m_queues; }

		void shutdown();


	private:

		ResultCode initialiseShaderManager();
		ResultCode initialiseGeometryManger();
		ResultCode initialiseTextureManager();
		ResultCode initialiseGuiManger();
		ResultCode initialiseCamera();
		ResultCode initialiseRenderQueues();

		ResultCode beginFrame(const RendererInputState& state);
		ResultCode executeFrame(const RendererInputState& state);
		ResultCode endFrame(const RendererInputState& state);

		void shutdownShaderManager();
		void shutdownGeometryManger();
		void shutdownTextureManager();
		void shutdownGuiManger();
		void shutdownCamera();
		void shutdownRenderQueues();


	private:
		ShaderManager m_shaders;
		GeometryManager m_geometry;
		TextureManager m_textures;
		GuiManager m_gui;
		Camera m_camera;
		RenderQueues m_queues;

		RenderStats m_renderStats;


	};
}
