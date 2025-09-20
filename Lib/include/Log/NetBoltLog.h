#pragma once
#include "pch.h"

namespace netblt
{
	// TODO: impl
	enum class LogLevel
	{
		LL_INFO,
		LL_DEBUG,
		LL_WARN,
		LL_ERROR,
		LL_FATAL
	};

	struct LogCreateInfo
	{
		std::string logFilePath = "AnvLog.log";
		std::string timeFormat = "%I:%M:%S";
		bool		consoleOutput = true;
		bool		fileOutput = false;
	};

	struct File;

	class NetBoltLog
	{
	public:
		static void Init(const LogCreateInfo& info);

		static void LOG_INFO (const std::string& str, ...);
		static void LOG_DEBUG(const std::string& str, ...);
		static void LOG_WARN (const std::string& str, ...);
		static void LOG_ERROR(const std::string& str, ...);
		static void LOG_FATAL(const std::string& str, ...);

		static std::string GetTime();

	private:
		static std::string formatString(const std::string& format, va_list args);

		static inline std::shared_ptr<File> m_File = nullptr;
		static inline LogCreateInfo m_CreationInfo = {};
	};
}

#define NB_INFO(fmt, ...)  netblt::NetBoltLog::LOG_INFO(fmt, ##__VA_ARGS__)
#define NB_DEBUG(fmt, ...) netblt::NetBoltLog::LOG_DEBUG(fmt, ##__VA_ARGS__)
#define NB_WARN(fmt, ...)  netblt::NetBoltLog::LOG_WARN(fmt, ##__VA_ARGS__)
#define NB_ERROR(fmt, ...) netblt::NetBoltLog::LOG_ERROR(fmt, ##__VA_ARGS__)
#define NB_FATAL(fmt, ...) netblt::NetBoltLog::LOG_FATAL(fmt, ##__VA_ARGS__)