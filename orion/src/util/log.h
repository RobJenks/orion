#pragma once

#include <string>
#include <mutex>
#include <sstream>
#include "result_code.h"

namespace Orion
{
	// Convenience macro to convert an input stream to a std::string
#	define T(x) { std::stringstream ss; ss << x; return ss.str(); }

	class Log
	{
	public:

		static ResultCode initialise(const std::string& target_file);

		static void info(const std::string& str);
		static void info(const std::string& str, const std::string & file, int line);
		static void warn(const std::string& str);
		static void warn(const std::string& str, const std::string& file, int line);
		static void error(const std::string& str);
		static void error(const std::string& str, const std::string& file, int line);

#		define LOG_INFO(x)  Log::info(x, __FILE__, __LINE__);
#		define LOG_WARN(x)  Log::warn(x, __FILE__, __LINE__);
#		define LOG_ERROR(x) Log::error(x, __FILE__, __LINE__);

	private:

		static void log_cat(const std::string & type, const std::string & str, const std::string & file, int line);
		static std::string trim_source_file(const std::string & file);

		static const std::string UNKNOWN_FILE;
		static const int UNKNOWN_LINE;
	private:

		static std::ofstream m_log;
		static std::mutex m_log_mutex;
		static bool m_log_initialised;

	};
}
