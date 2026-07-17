//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#include <Sentra/Core/Logger.h>

#include "spdlog/sinks/stdout_sinks.h"

namespace SCore {
    std::shared_ptr<spdlog::logger> Logger::s_Logger;
    std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

    void Logger::Init() {

        auto sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();

        sink->set_pattern("%^%T - %v%$");

        s_Logger = std::make_shared<spdlog::logger>("Sentra", sink);
        spdlog::register_logger(s_Logger);
        s_Logger->set_level(spdlog::level::trace);
        s_Logger->flush_on(spdlog::level::trace);

        s_ClientLogger = std::make_shared<spdlog::logger>("Client", sink);
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);

    }
}