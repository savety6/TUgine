#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>

class Log
{
private:
    /* data */
    static std::shared_ptr<spdlog::logger> coreLogger;
    static std::shared_ptr<spdlog::logger> clientLogger;

public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; }
};

#define TUG_CORE_TRACE(...) ::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TUG_CORE_INFO(...) ::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TUG_CORE_WARN(...) ::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TUG_CORE_ERROR(...) ::Log::GetCoreLogger()->error(__VA_ARGS__)
// #define TUG_CORE_FATAL(...) ::Log::GetCoreLogger()->e(__VA_ARGS__)

#define TUG_TRACE(...) ::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TUG_INFO(...) ::Log::GetClientLogger()->info(__VA_ARGS__)
#define TUG_WARN(...) ::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TUG_ERROR(...) ::Log::GetClientLogger()->error(__VA_ARGS__)
// #define TUG_FATAL(...) ::Log::GetClientLogger()->fatal(__VA_ARGS__)



