#pragma once

#include <memory.h>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Astra
{
	class ASTRA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

#if ASTRA_DEBUG
	// Core log macros
	#define ASTRA_CORE_TRACE(...)	::Astra::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define ASTRA_CORE_INFO(...)	::Astra::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define ASTRA_CORE_WARN(...)	::Astra::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define ASTRA_CORE_ERROR(...)	::Astra::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define ASTRA_CORE_FATAL(...)	::Astra::Log::GetCoreLogger()->critical(__VA_ARGS__)
#else
	// Release build stripped
	#define ASTRA_CORE_TRACE(...)
	#define ASTRA_CORE_INFO(...)
	#define ASTRA_CORE_WARN(...)
	#define ASTRA_CORE_ERROR(...)
	#define ASTRA_CORE_FATAL(...)
#endif

#if ASTRA_DEBUG
	// Client log macros
	#define ASTRA_TRACE(...)		::Astra::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define ASTRA_INFO(...)			::Astra::Log::GetClientLogger()->info(__VA_ARGS__)
	#define ASTRA_WARN(...)			::Astra::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define ASTRA_ERROR(...)		::Astra::Log::GetClientLogger()->error(__VA_ARGS__)
	#define ASTRA_FATAL(...)		::Astra::Log::GetClientLogger()->critical(__VA_ARGS__)
#else
	// Release build stripped
	#define ASTRA_TRACE(...)		::Astra::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define ASTRA_INFO(...)			::Astra::Log::GetClientLogger()->info(__VA_ARGS__)
	#define ASTRA_WARN(...)			::Astra::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define ASTRA_ERROR(...)		::Astra::Log::GetClientLogger()->error(__VA_ARGS__)
	#define ASTRA_FATAL(...)		::Astra::Log::GetClientLogger()->critical(__VA_ARGS__)
#endif