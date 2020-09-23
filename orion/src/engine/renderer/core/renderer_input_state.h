#pragma once

#include <bx/uint32_t.h>
#include "entry/entry.h"
#include "bx/math.h"

struct RendererInputState
{
	uint32_t							width;
	uint32_t							height;

	float								frame_ms;

	bx::Vec3							view_at;
	bx::Vec3							view_dir;

	const entry::MouseState *			mouse_state;
};
