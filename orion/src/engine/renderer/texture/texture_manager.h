#pragma once

#include "../../../util/result_code.h"

namespace Orion
{
	class TextureManager
	{
	public:

		TextureManager();

		ResultCode initialise();

		void shutdown();

	private:

	};
}
