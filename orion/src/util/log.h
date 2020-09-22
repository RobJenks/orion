#pragma once

#include <string>
#include <mutex>
#include <sstream>
#include "result_code.h"

namespace Orion
{
	// Convenience macro to convert an input stream to a std::string
#	define MAKE_TEXT(x) { std::stringstream ss; ss << x; return ss.str(); }

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

#		define LOG_CAT(x, fn) { std::stringstream ___ss; ___ss << x; fn(___ss.str(), __FILE__, __LINE__); } 

#		define LOG_INFO(x)  LOG_CAT(x, Log::info)
#		define LOG_WARN(x)  LOG_CAT(x, Log::warn)
#		define LOG_ERROR(x) LOG_CAT(x, Log::error)

#		define RETURN_LOG_ERROR(x, code) { LOG_CAT(x << " [" << code << ']', Log::error); return code; }


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
