#pragma once

#include <stdint.h>
#include "../../../util/result_code.h"
#include "../../../math/vec2.h"
#include "../shader/shader_manager.h"
#include "../geometry/geometry_manager.h"
#include "../texture/texture_manager.h"
#include "../gui/gui_manager.h"
#include "../debug/render_stats.h"
struct Args;

namespace Orion
{
	class Renderer
	{
	public:

		Renderer();

		ResultCode initialise(uint32_t width, uint32_t height, uint32_t debug, bool runtime_debug, uint32_t reset, Args args);

		ResultCode initialiseShaderManager();
		ResultCode initialiseGeometryManger();
		ResultCode initialiseTextureManager();
		ResultCode initialiseGuiManger();


		void shutdown();

		void shutdownShaderManager();
		void shutdownGeometryManger();
		void shutdownTextureManager();
		void shutdownGuiManger();
		


	private:
		ShaderManager m_shaders;
		GeometryManager m_geometry;
		TextureManager m_textures;
		GuiManager m_gui;

		RenderStats m_renderStats;
	};
}
