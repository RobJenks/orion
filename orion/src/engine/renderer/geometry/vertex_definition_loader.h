#pragma once

#include "../../../util/result_code.h"

namespace Orion
{
	template <typename T>
	class VertexDefinitionLoader
	{
	public:

		static ResultCode load()
		{
			T::init();
			// log("Loaded v def ...")

			return ResultCode::Success;
		}

	};
}
