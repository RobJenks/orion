#include "bgfx_utils.h"
#include "../core/renderer_input_state.h"
#include "../../../util/log.h"
#include "../../../util/debug.h"

#include "camera.h"

namespace Orion
{
	Camera::Camera()
		:
		m_mode(CameraMode::TopDown),
		m_fov(60.0f),
		m_near(0.1f),
		m_far(1000.0f),
		m_topdown_pos({ 0.0f, 0.0f }),
		m_topdown_height(200.0f)
	{
	}

	ResultCode Camera::initialise()
	{
		LOG_INFO("Initialising camera controller");

		return ResultCodes::Success;
	}

	ResultCode Camera::beginFrame(const RendererInputState& state)
	{
        float view[16], proj[16];

        // Set view and projection matrices, and default viewport, for the primary view
        calculateCameraTransforms(view, proj, state);
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

	void Camera::setFov(float fov)
	{
		ASS(fov > 0.0f && fov <= 360, "Invalid camera FOV: " << fov);
		m_fov = fov;
	}

	void Camera::setPlanes(float near, float far)
	{
		ASS(near > 0.0f && far > near, "Invalid camera plane configuration; near=" << near << ", far=" << far);
		m_near = near;
		m_far = far;
	}

    void Camera::setCameraMode(CameraMode mode)
    {
		m_mode = mode;
    }

    void Camera::setTopDownCameraPos(Vec2<float> pos)
    {
		m_topdown_pos = pos;
    }

    void Camera::moveTopDownCamera(Vec2<float> delta)
    {
		m_topdown_pos += delta;
    }

	void Camera::setTopDownCameraHeight(float height)
	{
		ASS(height > 0, "Invalid topdown camera height: " << m_topdown_height);
		m_topdown_height = height;
	}

    void Camera::shutdown()
	{
		LOG_INFO("Shutting down camera controller");
	}

	void Camera::calculateCameraTransforms(float *outView, float *outProj, const RendererInputState& state)
	{
        switch (m_mode)
        {
            case CameraMode::TopDown:
                calculateTopDownCameraTransforms(outView, outProj, state); break;
        }
	}

    void Camera::calculateTopDownCameraTransforms(float *outView, float *outProj, const RendererInputState& state)
    {
        bx::mtxLookAt(outView,
            bx::Vec3(m_topdown_pos.x, m_topdown_pos.y, -m_topdown_height),  // Eye vector
            bx::Vec3(m_topdown_pos.x, m_topdown_pos.y, 0.0f)                // Look-at point
        );

        bx::mtxProj(outProj, m_fov, float(state.width) / float(state.height), m_near, m_far, bgfx::getCaps()->homogeneousDepth);
    }
}
