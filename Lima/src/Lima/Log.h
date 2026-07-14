#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Lima {

	class LIMA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define L_CORE_TRACE(...)    ::Lima::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define L_CORE_INFO(...)     ::Lima::Log::GetCoreLogger()->info(__VA_ARGS__)
#define L_CORE_WARN(...)     ::Lima::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define L_CORE_ERROR(...)    ::Lima::Log::GetCoreLogger()->error(__VA_ARGS__)
#define L_CORE_CRITICAL(...)    ::Lima::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define L_TRACE(...)    ::Lima::Log::GetClientLogger()->trace(__VA_ARGS__)
#define L_INFO(...)     ::Lima::Log::GetClientLogger()->info(__VA_ARGS__)
#define L_WARN(...)     ::Lima::Log::GetClientLogger()->warn(__VA_ARGS__)
#define L_ERROR(...)    ::Lima::Log::GetClientLogger()->error(__VA_ARGS__)
#define L_CRITICAL(...)    ::Lima::Log::GetClientLogger()->critical(__VA_ARGS__)