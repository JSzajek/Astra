#include "Logger.h"

namespace Astra::Log
{
	Logger::Logger()
	{
		ResetLoggingImpl();
	}

	void Logger::ResetLoggingImpl()
	{
#if _DEBUG
		m_level = LogLevel::Error;
#else
		m_level = LogLevel::Info;
#endif
	}

	void Logger::SetLoggingLevelImpl(const LogLevel& level)
	{
		m_level = level;

		if (level == Error)
		{
			LogImpl(Warning, "Switching Logger to Error");
		}
		else
		{
			LogImpl(Warning, "Switching Logger to Warning");
		}
	}

	void Logger::LogImpl(LogLevel level, const char* message) const
	{
		if (m_level >= level)
		{
			std::cout << message << std::endl;
		}
	}
}