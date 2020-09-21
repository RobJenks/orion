#include "bgfx_utils.h"

#include "shader_manager.h"

namespace Orion
{
	ShaderManager::ShaderManager()
	{
	}

	ResultCode ShaderManager::initialise()
	{
		return ResultCodes::Success;
	}

	ResultCode ShaderManager::initialiseShaderPrograms()
	{
		RETURN_ON_ERROR(initialiseShaderProgram("colour", "vs_cubes", "fs_cubes"));
		RETURN_ON_ERROR(initialiseShaderProgram("inst_textured", "vs_instanced_texture", "fs_instanced_texture"));

		return ResultCodes::Success;
	}

	ResultCode ShaderManager::initialiseShaderProgram(const std::string& name, const std::string& vs, std::optional<std::string> fs)
	{
		// log (...)

		if (name.empty()) return ResultCodes::InvalidEmptyShaderName;
		if (m_shaders.find(name) != m_shaders.end()) return ResultCodes::CannotLoadDuplicateShader;

		auto fs_str = fs.has_value() ? fs.value().c_str() : NULL;
		auto program = loadProgram(vs.c_str(), fs_str);

		if (!bgfx::isValid(program))
		{
			return ResultCodes::CouldNotLoadShaderProgram;
		}

		m_shaders[name] = program;

		// log (...)
		return ResultCodes::Success;
	}

	void ShaderManager::shutdown()
	{
	}
}
