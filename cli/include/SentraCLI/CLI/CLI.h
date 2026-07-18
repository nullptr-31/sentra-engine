//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#ifndef SENTRA_ENGINE_CLI_H
#define SENTRA_ENGINE_CLI_H

#include <memory>

#include <Sentra/Core/Application.h>

#include "SentraCLI/CLI/CLIContext.h"
#include "SentraCLI/Command/CommandTrie.h"

namespace SCLI {
    class CLI {
    public:
        explicit CLI(const SCore::Application& application);

        void Run() const;

    private:
        void RegisterCommands() const;

        void RegisterExecCommands() const;

        void RegisterConfigCommands() const;

        void RegisterInterfaceConfigCommands() const;

        void HandleLine(const std::string& line) const;
        void PrintCandidates(const std::vector<CommandCandidate> &candidates) const;

    private:
        const SCore::Application& m_Application;

        std::unique_ptr<CLIContext> m_Context;
        std::unique_ptr<CommandTrie> m_CommandTrie;
    };
}

#endif //SENTRA_ENGINE_CLI_H
