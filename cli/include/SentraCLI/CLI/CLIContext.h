//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#ifndef SENTRA_ENGINE_CLICONTEXT_H
#define SENTRA_ENGINE_CLICONTEXT_H

#include <string>

namespace SCLI {
    enum class CLIMode {
        Exec,
        Config,
        InterfaceConfig,
    };

    class CLIContext {
    public:
        inline CLIMode GetCLIMode() const { return m_Mode; }
        void SetCLIMode(CLIMode mode);

        inline const std::string& GetCurrentInterface() const { return m_CurrentInterface; }
        void SetCurrentInterface(std::string interface);

        inline bool QuitRequested() const { return m_QuitRequested; }
        void RequestQuit();

        std::string GetPrompt() const;

    private:
        CLIMode m_Mode = CLIMode::Exec;

        std::string m_CurrentInterface;

        bool m_QuitRequested = false;
    };
} // SCLI

#endif //SENTRA_ENGINE_CLICONTEXT_H
