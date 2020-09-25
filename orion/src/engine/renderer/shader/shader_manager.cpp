#include <algorithm>
#include "bgfx_utils.h"
#include "../../../util/log.h"
#include "../core/renderer_input_state.h"

#include "shader_manager.h"

namespace Orion
{
	ShaderManager::ShaderManager()
	{
	}

	ResultCode ShaderManager::initialise()
	{
		LOG_INFO("Initialising shader manager");

		RETURN_ON_ERROR(initialiseShaderPrograms());
		RETURN_ON_ERROR(initialiseUniforms());

		return ResultCodes::Success;
	}

	ResultCode ShaderManager::initialiseShaderPrograms()
	{
		LOG_INFO("Initialising shader programs");

		RETURN_ON_ERROR(initialiseShaderProgram("colour", "vs_cubes", "fs_cubes"));
		RETURN_ON_ERROR(initialiseShaderProgram("inst_textured", "vs_instanced_texture", "fs_instanced_texture"));

		return ResultCodes::Success;
	}

	ResultCode ShaderManager::initialiseShaderProgram(const std::string& name, const std::string& vs, std::optional<std::string> fs)
	{
		LOG_INFO("Initialising shader program \"" << name << "\"");

		if (name.empty())
			RETURN_LOG_ERROR("Invalid empty shader name", ResultCodes::InvalidEmptyShaderName);
		if (m_shaders.find(name) != m_shaders.end())
			RETURN_LOG_ERROR("Cannot load duplicate shader; program \"" << name << "\" already exists", ResultCodes::CannotLoadDuplicateShader);

		auto fs_str = fs.has_value() ? fs.value().c_str() : NULL;
		const auto program = loadProgram(vs.c_str(), fs_str);

		if (!bgfx::isValid(program))
		{
			RETURN_LOG_ERROR("Could not load shader program \"" << name << "\"; invalid shader", ResultCodes::CouldNotLoadShaderProgram);
		}

		m_shaders[name] = program;

		LOG_INFO("Successfully loaded shader program \"" << name << "\"");
		return ResultCodes::Success;
	}

	ResultCode ShaderManager::initialiseUniforms()
	{
		LOG_INFO("Initialising uniform definitions");

		RETURN_ON_ERROR(createUniform("s_texColor", bgfx::UniformType::Enum::Sampler));

		return ResultCodes::Success;
	}

    ResultCode ShaderManager::createUniform(const std::string& name, bgfx::UniformType::Enum type, uint16_t num)
    {
		if (m_uniforms.find(name) != m_uniforms.end())
		{
			RETURN_LOG_ERROR("Cannot create uniform definition; uniform already exists with name \"" << name << "\"", ResultCodes::CannotCreateDuplicateUniform);
		}

		auto uniform = bgfx::createUniform(name.c_str(), type, num);
		if (!bgfx::isValid(uniform))
		{
			RETURN_LOG_ERROR("Failed to create uniform \"" << name << "\"", ResultCodes::FailedToCreateUniform);
		}

		m_uniforms[name] = uniform;
		return ResultCodes::Success;
    }

	ResultCode ShaderManager::beginFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	ResultCode ShaderManager::executeFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	ResultCode ShaderManager::endFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	void ShaderManager::shutdown()
	{
		LOG_INFO("Shutting down shader manager");

		shutdownShaderPrograms();
        shutdownUniforms();
	}

	void ShaderManager::shutdownShaderPrograms()
	{
		LOG_INFO("Releasing all shader programs");

		std::for_each(m_shaders.begin(), m_shaders.end(), [](auto& shader) { bgfx::destroy(shader.second); });
		m_shaders.clear();
	}

	void ShaderManager::shutdownUniforms()
	{
        LOG_INFO("Releasing all uniform definitions");

        std::for_each(m_uniforms.begin(), m_uniforms.end(), [](auto& uniform) { bgfx::destroy(uniform.second); });
        m_uniforms.clear();
	}
}
