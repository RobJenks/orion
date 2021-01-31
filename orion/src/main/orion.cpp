#include <bx/uint32_t.h>
#include <entry/input.h>
#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"
#include "../engine/renderer/core/renderer_input_state.h"
#include "../container/container.h"
#include "../engine/renderer/geometry/geometry_manager.h"
#include "../engine/renderer/shader/shader_manager.h"
#include "../engine/renderer/texture/texture_manager.h"
#include "../util/log.h"

#include "orion.h"

namespace Orion
{
    Orion::Orion(const char* name, const char* description, const char* url)
        :
        entry::AppI(name, description, url),
        m_width(0U),
        m_height(0U),
        m_debug(0U),
        m_reset(0U),
		m_renderer(),

		tmp_data(Vec2<Container::Coord>(10, 10)),
		tmp_pos({ 0,0 })
    {
    }

    void Orion::init(int32_t argc, const char* const* argv, uint32_t width, uint32_t height)
    {
		// Temporary
		Log::initialise("orion.log");

        Args args(argc, argv);

		const uint32_t debug = BGFX_DEBUG_TEXT;
		const uint32_t reset = BGFX_RESET_VSYNC;

		const auto rendererInit = m_renderer.initialise(width, height, debug, RENDERER_RUNTIME_DEBUG_ENABLED, reset, args);
		if (ResultCodes::isError(rendererInit))
		{
			LOG_ERROR("Fatal error initialising renderer (" << rendererInit << "), cannot continue");
			exit(1);
		}

		// Temporary
		const auto tiles = std::vector<Tile>({
			Tile(1, Dir4::UP, { 1,1 }),
			Tile(1, Dir4::UP, { 2,1 }),
			Tile(1, Dir4::UP, { 3,1 }),
			Tile(1, Dir4::UP, { 3,2 }),
			Tile(1, Dir4::UP, { 3,3 }),
			Tile(1, Dir4::UP, { 4,3 }),
			Tile(1, Dir4::UP, { 5,3 }),
		});

		std::for_each(tiles.cbegin(), tiles.cend(), [&](const Tile& tile) {
			ASS(tmp_data.addTile(tile), "Add failed");
		});
//		tmp_data.addTileUnchecked(Tile(12, Dir4::RIGHT, { 4,4 }));

    }

    int Orion::shutdown()
    {
		// Shutdown primary components
		m_renderer.shutdown();

        return 0;
    }

    bool Orion::update()
    {
        if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
		{
			RendererInputState renderState;
			renderState.width = m_width;
			renderState.height = m_height;
			renderState.mouse_state = &m_mouseState;
			
			if (!_captureTemporaryInput(renderState)) return false;
			
			_renderTemporaryCube();
			_renderTemporaryTiles(renderState);
						
			m_renderer.frame(renderState);

            return true;
        }

        return false;
    }

	// Temporary
	bool Orion::_captureTemporaryInput(const RendererInputState& state)
	{
		if (inputGetKeyState(entry::Key::Esc, nullptr)) return false;

		const float BASE_MOVE = 100000.0f;
		const float BASE_ZOOM = 10.0f;

		uint8_t modifiers;
		Vec2<float> delta(0.0f, 0.0f);

		if (inputGetKeyState(entry::Key::KeyW, &modifiers)) delta += Vec2<float>(0.0f, +1.0f * _getTemporaryMoveDelta(BASE_MOVE, modifiers));
		if (inputGetKeyState(entry::Key::KeyS, &modifiers)) delta += Vec2<float>(0.0f, -1.0f * _getTemporaryMoveDelta(BASE_MOVE, modifiers));
		if (inputGetKeyState(entry::Key::KeyA, &modifiers)) delta += Vec2<float>(-1.0f * _getTemporaryMoveDelta(BASE_MOVE, modifiers), 0.0f);
		if (inputGetKeyState(entry::Key::KeyD, &modifiers)) delta += Vec2<float>(+1.0f * _getTemporaryMoveDelta(BASE_MOVE, modifiers), 0.0f);
		
		tmp_pos += delta;
		m_renderer.getCamera().moveTopDownCamera(delta);

		static int32_t last_mouse_wheel = 0;
		auto wheel_delta = state.mouse_state->m_mz - last_mouse_wheel;
		if (wheel_delta > 0) m_renderer.getCamera().adjustTopDownCameraHeight(-1.0f * BASE_ZOOM);
		if (wheel_delta < 0) m_renderer.getCamera().adjustTopDownCameraHeight(+1.0f * BASE_ZOOM);
		last_mouse_wheel = state.mouse_state->m_mz;
		
		return true;
	}

	float Orion::_getTemporaryMoveDelta(float base, uint8_t modifiers)
	{
		const float frame_pc = float(m_renderer.getRenderStats().getFrameMs()) * (1.0f / 1000.0f);
		const float speed_mult = (modifiers & entry::Modifier::LeftShift ? 10.0f : 1.0f);

		return base * speed_mult * frame_pc;
	}

	// Temporary
	void Orion::_renderTemporaryCube()
	{
		const auto shader = m_renderer.getShaderManager().getProgram("colour");
		const auto mesh = m_renderer.getGeometryManager().getMesh("cube");

		static float timeRot = 0.0f;
		timeRot += float(bgfx::getStats()->cpuTimeFrame) * (1000.0f / float(bgfx::getStats()->cpuTimerFreq)) * 0.001f;
		
		uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;

		float rot[16], scale[16], world[16];
		bx::mtxRotateXY(rot, 0.42f + timeRot, 0.74f + timeRot);
		bx::mtxScale(scale, 8.0f);
		bx::mtxMul(world, scale, rot);

		m_renderer.submitImmediate(RenderConfig(shader, mesh.vertex_buffer, mesh.index_buffer, state), world);
	}

	// Temporary
	void Orion::_renderTemporaryTiles(const RendererInputState & renderer_state)
	{
		const auto shader = m_renderer.getShaderManager().getProgram("inst_textured");
		const auto mesh = m_renderer.getGeometryManager().getMesh("quad");
		const auto uniform = m_renderer.getShaderManager().getUniform("s_texColor");
		const auto texture = m_renderer.getTextureManager().getTexture("fieldstone");
		uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;
		RenderConfig config(shader, mesh.vertex_buffer, mesh.index_buffer, state, RenderConfig::Textures(TextureUniformBinding(texture, uniform)));

		InstanceData inst;
		float scale[16], trans[16];
		bx::mtxScale(scale, 10.0f);

		if (renderer_state.width == 121212121) std::cout << "";

		//for (int i = 0; i < 4; ++i)
		//{
		//	bx::mtxTranslate(trans, (i == 0) ? -36.0f : 35.0f, 18.0f, 0);
		//	//bx::mtxTranslate(trans, 0.0f + (float(i) * 20.0f), 15.0f, 0.0f);
		//	bx::mtxMul(inst.transform, scale, trans);

		//	m_renderer.queue().primary().submit(config, inst);
		//}

		const auto & tiles = tmp_data.getTiles();
		for (const auto& tile : tiles)
		{
			bx::mtxTranslate(trans, (float(tile.getLocation().x) * 20.0f), (float(tile.getLocation().y) * 20.0f), 0.0f);
			bx::mtxMul(inst.transform, scale, trans);

			m_renderer.queue().primary().submit(config, inst);
		}
	}
}

ENTRY_IMPLEMENT_MAIN(
    Orion::Orion
    , "orion"
    , "orion"
    , ""
);
