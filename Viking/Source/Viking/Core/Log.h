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

        static Ref<spdlog::logger>& getCoreLogger() {
            return sCoreLogger;
        };
        static Ref<spdlog::logger>& getClientLogger() {
            return sClientLogger;
        };

    private:
        static Ref<spdlog::logger> sCoreLogger;
        static Ref<spdlog::logger> sClientLogger;
    };
}

// Core log macros
#define VI_CORE_TRACE(...)    ::Viking::Log::getCoreLogger()->trace(__VA_ARGS__)
#define VI_CORE_INFO(...)     ::Viking::Log::getCoreLogger()->info(__VA_ARGS__)
#define VI_CORE_WARN(...)     ::Viking::Log::getCoreLogger()->warn(__VA_ARGS__)
#define VI_CORE_ERROR(...)    ::Viking::Log::getCoreLogger()->error(__VA_ARGS__)
#define VI_CORE_CRITICAL(...) ::Viking::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define VI_TRACE(...)         ::Viking::Log::getClientLogger()->trace(__VA_ARGS__)
#define VI_INFO(...)          ::Viking::Log::getClientLogger()->info(__VA_ARGS__)
#define VI_WARN(...)          ::Viking::Log::getClientLogger()->warn(__VA_ARGS__)
#define VI_ERROR(...)         ::Viking::Log::getClientLogger()->error(__VA_ARGS__)
#define VI_CRITICAL(...)      ::Viking::Log::getClientLogger()->critical(__VA_ARGS__)

#endif //VIKING_LOG_H
