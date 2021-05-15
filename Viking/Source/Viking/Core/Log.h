//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_LOG_H
#define VIKING_LOG_H

#include "Viking/Core/Base.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Viking {
    class Log {
    public:
        static void init();

        static Ref<spdlog::logger>& getCoreLogger();
        static Ref<spdlog::logger>& getClientLogger();

    private:
        static Ref<spdlog::logger> sCoreLogger;
        static Ref<spdlog::logger> sClientLogger;
    };
}

// Core log macros
#define VI_CORE_TRACE(...)    ::Viking::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VI_CORE_INFO(...)     ::Viking::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VI_CORE_WARN(...)     ::Viking::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VI_CORE_ERROR(...)    ::Viking::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VI_CORE_CRITICAL(...) ::Viking::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define VI_TRACE(...)         ::Viking::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VI_INFO(...)          ::Viking::Log::GetClientLogger()->info(__VA_ARGS__)
#define VI_WARN(...)          ::Viking::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VI_ERROR(...)         ::Viking::Log::GetClientLogger()->error(__VA_ARGS__)
#define VI_CRITICAL(...)      ::Viking::Log::GetClientLogger()->critical(__VA_ARGS__)

#endif //VIKING_LOG_H
