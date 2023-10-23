#pragma once

#include "spdlog/spdlog.h"

namespace TUgine
{
    class Log
    {
    public:
        Log();
        ~Log();

        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return _coreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _clientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> _coreLogger;
        static std::shared_ptr<spdlog::logger> _clientLogger;
    };
} 
// core log macros
#define TUG_CORE_TRACE(...) ::TUgine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TUG_CORE_INFO(...) ::TUgine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TUG_CORE_WARN(...) ::TUgine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TUG_CORE_ERROR(...) ::TUgine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TUG_CORE_FATAL(...) ::TUgine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// client log macros
#define TUG_TRACE(...) ::TUgine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TUG_INFO(...) ::TUgine::Log::GetClientLogger()->info(__VA_ARGS__)
#define TUG_WARN(...) ::TUgine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TUG_ERROR(...) ::TUgine::Log::GetClientLogger()->error(__VA_ARGS__)
#define TUG_FATAL(...) ::TUgine::Log::GetClientLogger()->fatal(__VA_ARGS__)
