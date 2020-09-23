#include "bgfx_utils.h"
#include "../core/renderer_input_state.h"
#include "../../../util/log.h"

#include "camera.h"

namespace Orion
{
	ResultCode Camera::initialise()
	{
		LOG_INFO("Initialising camera controller");

		return ResultCodes::Success;
	}

	ResultCode Camera::beginFrame(const RendererInputState& state)
	{
		// Set view and projection matrices, and default viewport, for the primary view
		float view[16];
		bx::mtxLookAt(view, state.view_dir, state.view_at);

		float proj[16];
		bx::mtxProj(proj, 60.0f, float(state.width) / float(state.height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		bgfx::setViewRect(0, 0, 0, uint16_t(state.width), uint16_t(state.height));

		return ResultCodes::Success;
	}

	ResultCode Camera::executeFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	ResultCode Camera::endFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	void Camera::shutdown()
	{
		LOG_INFO("Shutting down camera controller");
	}
}
