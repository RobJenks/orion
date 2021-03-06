#pragma once

#include "common.h"
#include "../engine/renderer/core/renderer.h"

// Temporary
#include "../container/container.h"

namespace Orion
{
    class Orion : public entry::AppI
    {
    public:
#		ifdef _DEBUG
		static const bool RENDERER_RUNTIME_DEBUG_ENABLED = true;
#		else
		static const bool RENDERER_RUNTIME_DEBUG_ENABLED = false;
#		endif

        Orion(const char* name, const char* description, const char* url);

        void init(int32_t argc, const char* const* argv, uint32_t width, uint32_t height) override;

        virtual int shutdown() override;

        bool update() override;

	private:

		bool _captureTemporaryInput(const RendererInputState& state);
		float _getTemporaryMoveDelta(float base, uint8_t modifiers);
		void _renderTemporaryCube();
		void _renderTemporaryTiles(const RendererInputState & state);

    private:

        entry::MouseState m_mouseState;

        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_debug;
        uint32_t m_reset;

		Renderer m_renderer;

		// Temporary
		Container tmp_data;
		Vec2<float> tmp_pos;
    };
};
