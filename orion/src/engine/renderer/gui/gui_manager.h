#pragma once

#include "../../../util/result_code.h"

namespace Orion
{
	class GuiManager
	{
	public:

		GuiManager();

		ResultCode initialise();

		void shutdown();

	private:
	};
}
