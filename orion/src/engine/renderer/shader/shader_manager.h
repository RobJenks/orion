#pragma once

#include <string>
#include <optional>
#include <unordered_map>
#include "../../../util/result_code.h"
#include "bgfx_utils.h"

namespace Orion
{
	class ShaderManager
	{
	public:

		ShaderManager();

		ResultCode initialise();

		ResultCode initialiseShaderPrograms();
		ResultCode initialiseShaderProgram(const std::string & name, const std::string & vs, std::optional<std::string> fs);


		void shutdown();

	private:

		std::unordered_map<std::string, bgfx::ProgramHandle> m_shaders;

	};
}
