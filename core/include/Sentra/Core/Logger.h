//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#ifndef SENTRA_ENGINE_LOGGER_H
#define SENTRA_ENGINE_LOGGER_H

#include <iostream>
#include <memory>

#include <spdlog/spdlog.h>

namespace SCore {

    class Logger {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger> &GetLogger() {
            if (s_Logger == nullptr)
                std::cerr << "Make sure to call Logger::Init()" << std::endl;
            return s_Logger;
        }

        static std::shared_ptr<spdlog::logger> &GetClientLogger() {
            if (s_ClientLogger == nullptr)
                std::cerr << "Make sure to call Logger::Init()" << std::endl;
            return s_ClientLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> s_Logger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

#define SCORE_TRACE(...) ::SCore::Logger::GetLogger()->trace(__VA_ARGS__)
#define SCORE_DEBUG(...) ::SCore::Logger::GetLogger()->debug(__VA_ARGS__)
#define SCORE_INFO(...) ::SCore::Logger::GetLogger()->info(__VA_ARGS__)
#define SCORE_WARN(...) ::SCore::Logger::GetLogger()->warn(__VA_ARGS__)
#define SCORE_ERROR(...) ::SCore::Logger::GetLogger()->error(__VA_ARGS__)
#define SCORE_CRITICAL(...) \
::SCore::Logger::GetLogger()->critical(__VA_ARGS__)

#define S_TRACE(...) ::SCore::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define S_DEBUG(...) ::SCore::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define S_INFO(...) ::SCore::Logger::GetClientLogger()->info(__VA_ARGS__)
#define S_WARN(...) ::SCore::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define S_ERROR(...) ::SCore::Logger::GetClientLogger()->error(__VA_ARGS__)
#define S_CRITICAL(...) \
::SCore::Logger::GetClientLogger()->critical(__VA_ARGS__)

#endif //SENTRA_ENGINE_LOGGER_H
