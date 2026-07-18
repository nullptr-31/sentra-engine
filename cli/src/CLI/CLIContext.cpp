//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#include "SentraCLI/CLI/CLIContext.h"

namespace SCLI {
    void CLIContext::SetCLIMode(const CLIMode mode) {
        m_Mode = mode;
    }

    void CLIContext::SetCurrentInterface(std::string interface) {
        m_CurrentInterface = std::move(interface);
    }

    void CLIContext::RequestQuit() {
        m_QuitRequested = true;
    }

    std::string CLIContext::GetPrompt() const {
        switch (m_Mode) {
            case CLIMode::Exec:
                return "Sentra# ";
            case CLIMode::Config:
                return "Sentra(config)# ";
            case CLIMode::InterfaceConfig:
                return "Sentra(config-if)# ";
        }
        return "Sentra# ";
    }
} // SCLI