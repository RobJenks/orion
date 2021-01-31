#pragma once

#include <bx/uint32_t.h>
#include "entry/entry.h"
#include "bx/math.h"

struct RendererInputState
{
	uint32_t							width;
	uint32_t							height;

	float								frame_ms;

	const entry::MouseState *			mouse_state;
};
