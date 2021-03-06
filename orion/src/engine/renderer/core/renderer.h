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
		
		const inline ShaderManager& getShaderManager() const { return m_shaders; }
		const inline GeometryManager& getGeometryManager() const { return m_geometry; }
		const inline TextureManager& getTextureManager() const { return m_textures; }
		const inline GuiManager& getGuiManager() const { return m_gui; }
		const inline Camera& getCamera() const { return m_camera; }

		inline RenderQueues& queue() { return m_queues; }
		const inline RenderQueues& queue() const { return m_queues; }

		inline const RenderStats& getRenderStats() { return m_renderStats; }

		void submitImmediate(const RenderConfig& config);
		void submitImmediate(const RenderConfig& config, float transform[16]);
		

		void shutdown();


	private:

		ResultCode initialiseShaderManager();
		ResultCode initialiseGeometryManger();
		ResultCode initialiseTextureManager();
		ResultCode initialiseGuiManger();
		ResultCode initialiseCamera();
		ResultCode initialiseRenderQueues();
		ResultCode initialiseRenderStats();

		ResultCode beginFrame(const RendererInputState& state);
		ResultCode executeFrame(const RendererInputState& state);
		ResultCode endFrame(const RendererInputState& state);

		ResultCode render(const RendererInputState& state);

		ResultCode processRenderQueues(const RendererInputState& state);
		template <typename T>
		ResultCode processRenderQueue(const RenderQueue<T>& queue, const RendererInputState& state);
		void submitWithRenderConfig(const RenderConfig& config);

		ResultCode resetRenderQueues();
		template <typename T>
		ResultCode resetRenderQueue(RenderQueue<T>& queue);

		void renderDebugInfo();

		void shutdownShaderManager();
		void shutdownGeometryManger();
		void shutdownTextureManager();
		void shutdownGuiManger();
		void shutdownCamera();
		void shutdownRenderQueues();
		void shutdownRenderStats();


	private:
		ShaderManager m_shaders;
		GeometryManager m_geometry;
		TextureManager m_textures;
		GuiManager m_gui;
		Camera m_camera;
		RenderQueues m_queues;

		RenderStats m_renderStats;


	};
	template<typename T>
	inline ResultCode Renderer::processRenderQueue(const RenderQueue<T>& queue, const RendererInputState& state)
	{
		(void)state;	// Current unused
		bgfx::InstanceDataBuffer instanceBuffer;

		for (const RenderSlot<T>& slot : queue.getSlots())
		{
			const std::vector<T>& instances = slot.getInstances();
			const uint32_t count = static_cast<uint32_t>(instances.size());
			if (count == 0U) continue;

			if (count != bgfx::getAvailInstanceDataBuffer(count, sizeof(T)))
			{
				return ResultCodes::CannotAllocateSufficientlyLargeInstanceBuffer;
			}

			// Build and assign instance buffer
			bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(T));
			memcpy((void*)instanceBuffer.data, (const void*)instances.data(), count * sizeof(T));
			bgfx::setInstanceDataBuffer(&instanceBuffer);

			/*float id[16];
			bx::mtxIdentity(id);
			bgfx::setTransform(id);*/

			// Submit draw call
			submitWithRenderConfig(slot.getConfig());
		}

		return ResultCodes::Success;
	}

	template<typename T>
	inline ResultCode Renderer::resetRenderQueue(RenderQueue<T>& queue)
	{
		return queue.reset();
	}
}
