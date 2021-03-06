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

		ResultCode beginFrame(const RendererInputState& state);
		ResultCode executeFrame(const RendererInputState& state);
		ResultCode endFrame(const RendererInputState& state);

		inline bgfx::ProgramHandle getProgram(const std::string& name) const { return m_shaders.at(name); }
		inline bgfx::UniformHandle getUniform(const std::string& name) const { return m_uniforms.at(name); }

		void shutdown();

	private:

		ResultCode initialiseShaderPrograms();
		ResultCode initialiseShaderProgram(const std::string& name, const std::string& vs, std::optional<std::string> fs);
		ResultCode initialiseUniforms();

		ResultCode createUniform(const std::string& name, bgfx::UniformType::Enum type, uint16_t num = (uint16_t)1U);

		void shutdownShaderPrograms();
		void shutdownUniforms();

	private:

		std::unordered_map<std::string, bgfx::ProgramHandle> m_shaders;
		std::unordered_map<std::string, bgfx::UniformHandle> m_uniforms;

	};
}
