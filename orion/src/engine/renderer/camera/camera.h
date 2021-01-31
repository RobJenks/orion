#pragma once

#include "../../../util/result_code.h"
#include "../../../math/vec2.h"
#include "camera_mode.h"
struct RendererInputState;

namespace Orion
{
	class Camera
	{
	public:

		Camera();

		ResultCode initialise();

		ResultCode beginFrame(const RendererInputState& state);
		ResultCode executeFrame(const RendererInputState& state);
		ResultCode endFrame(const RendererInputState& state);

		inline float getFov() const { return m_fov; }
		inline float getNear() const { return m_near; }
		inline float getFar() const { return m_far; }

		void setFov(float fov);
		void setPlanes(float near, float far);

		inline CameraMode getCameraMode() const { return m_mode; }
		void setCameraMode(CameraMode mode);

		inline Vec2<float> getTopDownCameraPos() const { return m_topdown_pos; }
		inline float getTopDownCameraHeight() const { return m_topdown_height; }

		void setTopDownCameraPos(Vec2<float> pos);
		void moveTopDownCamera(Vec2<float> delta);
		void setTopDownCameraHeight(float height);

		void shutdown();

	private:

		void calculateCameraTransforms(float* outView, float* outProj, const RendererInputState& state);
		void calculateTopDownCameraTransforms(float* outView, float* outProj, const RendererInputState& state);

	private:

		float m_fov;
		float m_near;
		float m_far;

		CameraMode m_mode;

		Vec2<float> m_topdown_pos;
		float m_topdown_height;
	};
}
