#pragma once

#include <iostream>
#include <string>

namespace Astra::Log
{
	enum LogLevel : unsigned char
	{
		Info = 0,
		Warning = 1,
		Error = 2,
	};

	class Logger
	{
	private:
		LogLevel m_level;
	public:
		Logger(const Logger&) = delete;
		void operator=(const Logger&) = delete;

		static Logger& Get()
		{
			static Logger instance;
			return instance;
		}

		static void const LogError(const std::string& message)
		{
			Get().LogImpl(Error, message.c_str());
		}

		static void const LogError(const char* message)
		{
			Get().LogImpl(Error, message);
		}

		static void const LogWarning(const std::string& message)
		{
			Get().LogImpl(Warning, message.c_str());
		}

		static void const LogWarning(const char* message)
		{
			Get().LogImpl(Warning, message);
		}

		static void const Log(const std::string& message)
		{
			Get().LogImpl(Info, message.c_str());
		}

		static void const Log(const char* message)
		{
			Get().LogImpl(Info, message);
		}

		static void ResetLogging()
		{
			Get().ResetLoggingImpl();
		}

		static void SetLoggingLevel(const LogLevel& level) 
		{
			Get().SetLoggingLevelImpl(level);
		}

	private:
		Logger();
		void SetLoggingLevelImpl(const LogLevel& level);
		void ResetLoggingImpl();
		void LogImpl(LogLevel level, const char* message) const;
	};
}