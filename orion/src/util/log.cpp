#include <filesystem>
#include <fstream>
#include "log.h"
namespace fs = std::filesystem;

namespace Orion
{
	const std::string Log::UNKNOWN_FILE = "unknowm";
	const int Log::UNKNOWN_LINE = 0;

	bool Log::m_log_initialised = false;
	std::ofstream Log::m_log;
	std::mutex Log::m_log_mutex;

	ResultCode Log::initialise(const std::string& target_file)
	{
		std::lock_guard<std::mutex> lock(m_log_mutex);

		if (m_log_initialised)
		{
			m_log.close();
		}

		fs::path path(target_file);
		m_log = std::ofstream(path.c_str(), std::ios::out | std::ios::trunc);
		m_log_initialised = true;

		return ResultCodes::Success;
	}

	void Log::info(const std::string& str)
	{
		info(str, UNKNOWN_FILE, UNKNOWN_LINE);
	}

	void Log::info(const std::string& str, const std::string & file, int line)
	{
		log_cat("INFO", str, file, line);
	}

	void Log::warn(const std::string& str)
	{
		warn(str, UNKNOWN_FILE, UNKNOWN_LINE);
	}

	void Log::warn(const std::string& str, const std::string& file, int line)
	{
		log_cat("WARN", str, file, line);
	}

	void Log::error(const std::string& str)
	{
		error(str, UNKNOWN_FILE, UNKNOWN_LINE);
	}

	void Log::error(const std::string& str, const std::string& file, int line)
	{
		log_cat("ERROR", str, file, line);
	}


	void Log::log_cat(const std::string & type, const std::string & str, const std::string & file, int line)
	{
		std::lock_guard<std::mutex> lock(m_log_mutex);

		const auto time = std::time(nullptr);
		m_log << "[" << std::put_time(std::gmtime(&time), "%F %T") << "] " << type << " "
			<< trim_source_file(file) << ":" << line << " - " << str << std::endl;
		m_log.flush();
	}

	std::string Log::trim_source_file(const std::string & file)
	{
		fs::path full_path(file);
		return full_path.filename().string();
	}
}
