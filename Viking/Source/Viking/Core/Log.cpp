//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Viking {
    Ref<spdlog::logger> Log::sCoreLogger;
    Ref<spdlog::logger> Log::sClientLogger;

    void Log::init() {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Viking.log", true));

        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        sCoreLogger = std::make_shared<spdlog::logger>("Viking", begin(logSinks), end(logSinks));
        spdlog::register_logger(sCoreLogger);
        sCoreLogger->set_level(spdlog::level::trace);
        sCoreLogger->flush_on(spdlog::level::trace);

        sClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(sClientLogger);
        sClientLogger->set_level(spdlog::level::trace);
        sClientLogger->flush_on(spdlog::level::trace);
    }
}
