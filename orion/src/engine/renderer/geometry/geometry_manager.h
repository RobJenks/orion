#pragma once

#include "../../../util/result_code.h"
struct RendererInputState;

namespace Orion
{
	class GeometryManager
	{
	public:

		GeometryManager();

		ResultCode initialise();
		ResultCode initialiseVertexDefinitions();

		ResultCode beginFrame(const RendererInputState& state);
		ResultCode executeFrame(const RendererInputState& state);
		ResultCode endFrame(const RendererInputState& state);

		void shutdown();

	private:

	};
}
