#pragma once

#include "bgfx_utils.h"

namespace Orion
{
	class BgfxSupport
	{
	public:

		static constexpr inline const uint16_t INVALID_HANDLE = UINT16_MAX;

		// Constexpr version of handle validity check
		template <typename THandle>
		constexpr inline static const bool isValid(THandle handle)
		{
			return handle.idx != INVALID_HANDLE;
		}

	};
}
