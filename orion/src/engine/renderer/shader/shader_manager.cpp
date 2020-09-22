#include "bgfx_utils.h"
#include "../../../util/log.h"

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
		auto program = loadProgram(vs.c_str(), fs_str);

		if (!bgfx::isValid(program))
		{
			RETURN_LOG_ERROR("Could not load shader program \"" << name << "\"; invalid shader", ResultCodes::CouldNotLoadShaderProgram);
		}

		m_shaders[name] = program;

		LOG_INFO("Successfully loaded shader program \"" << name << "\"");
		return ResultCodes::Success;
	}

	void ShaderManager::shutdown()
	{
		LOG_INFO("Shutting down shader manager");
	}
}
