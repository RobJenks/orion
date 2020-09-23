#pragma once

#include "../../../util/result_code.h"
struct RendererInputState;

namespace Orion
{
	class GuiManager
	{
	public:

		GuiManager();

		ResultCode initialise();

		ResultCode beginFrame(const RendererInputState& state);
		ResultCode executeFrame(const RendererInputState& state);
		ResultCode endFrame(const RendererInputState& state);

		void shutdown();

	private:
	};
}
