//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#ifndef SENTRA_ENGINE_APPLICATION_H
#define SENTRA_ENGINE_APPLICATION_H
#include "Sentra/Capture/CaptureManager.h"
#include "Sentra/Network/InterfaceManager.h"

namespace SCore {
    class Application {
    public:
        Application();

        ~Application();

        void Run();

        void Stop();

        inline bool IsRunning() const { return m_IsRunning; }

        inline InterfaceManager& GetInterfaceManager() const { return m_InterfaceManager; }
        inline CaptureManager& GetCaptureManager() const { return m_CaptureManager; }

    private:
        InterfaceManager& m_InterfaceManager;
        CaptureManager& m_CaptureManager;

        bool m_IsRunning;
    };
}

#endif //SENTRA_ENGINE_APPLICATION_H
