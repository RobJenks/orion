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
		



		inline static constexpr bool isSuccess(ResultCode code) { return code == 0; }
		inline static constexpr bool isError(ResultCode code) { return code != 0; }
	};
#	undef add

#	define RETURN_ON_ERROR(x) { const ResultCode __rc = x; if (ResultCodes::isError(__rc)) { return __rc; }}
}
