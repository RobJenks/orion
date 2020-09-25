#pragma once

namespace Orion
{
	typedef int ResultCode;
#	define add(id, name) static const ResultCode name = id;


	class ResultCodes
	{
	public:
		add(0, Success);
		add(1, UnspecifiedError);

		add(100, CouldNotInitEngineLibrary);
		add(101, CouldNotLoadShaderProgram);
		add(102, InvalidEmptyShaderName);
		add(103, CannotLoadDuplicateShader);
		add(104, CannotLoadMeshWithInvalidEmptyName);
		add(105, CannotLoadMeshWithInvalidName);
		add(106, CannotStoreDuplicateMesh);
		add(107, CannotCreateDuplicateUniform);
		add(108, FailedToCreateUniform);
		



		inline static constexpr bool isSuccess(ResultCode code) { return code == 0; }
		inline static constexpr bool isError(ResultCode code) { return code != 0; }
	};
#	undef add

#	define RETURN_ON_ERROR(x) { const ResultCode __rc = x; if (ResultCodes::isError(__rc)) { return __rc; }}
}
