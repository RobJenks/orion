#pragma once

#include "../../../util/result_code.h"

namespace Orion
{
	class GeometryManager
	{
	public:

		GeometryManager();

		ResultCode initialise();

		ResultCode initialiseVertexDefinitions();


		void shutdown();

	private:

	};
}
