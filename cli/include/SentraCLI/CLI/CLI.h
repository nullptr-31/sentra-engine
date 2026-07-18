//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#ifndef SENTRA_ENGINE_CLI_H
#define SENTRA_ENGINE_CLI_H

#include <memory>

#include <Sentra/Core/Application.h>

#include "SentraCLI/CLI/CLIContext.h"

namespace SCLI {
    class CLI {
    public:
        explicit CLI(const SCore::Application& application);

        void Run() const;

    private:
        void HandleLine(std::string_view line) const;

    private:
        const SCore::Application& m_Application;

        std::unique_ptr<CLIContext> m_Context;
    };
}

#endif //SENTRA_ENGINE_CLI_H
