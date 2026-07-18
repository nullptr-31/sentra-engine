//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#ifndef SENTRA_ENGINE_CLI_H
#define SENTRA_ENGINE_CLI_H

#include "Sentra/Core/Application.h"

namespace SCLI {
    class CLI {
    public:
        explicit CLI(const SCore::Application& application);

        void Run() const;
    private:
        const SCore::Application& m_Application;
    };
}

#endif //SENTRA_ENGINE_CLI_H
