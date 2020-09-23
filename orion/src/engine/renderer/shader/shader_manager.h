#pragma once

#include <string>
#include <optional>
#include <unordered_map>
#include "../../../util/result_code.h"
#include "bgfx_utils.h"
struct RendererInputState;

namespace Orion
{
	class ShaderManager
	{
	public:

		ShaderManager();

		ResultCode initialise();

		ResultCode initialiseShaderPrograms();
		ResultCode initialiseShaderProgram(const std::string & name, const std::string & vs, std::optional<std::string> fs);

		ResultCode beginFrame(const RendererInputState& state);
		ResultCode executeFrame(const RendererInputState& state);
		ResultCode endFrame(const RendererInputState& state);

		inline bgfx::ProgramHandle getProgram(const std::string& name) { return m_shaders[name]; }


		void shutdown();

	private:

		std::unordered_map<std::string, bgfx::ProgramHandle> m_shaders;

	};
}
