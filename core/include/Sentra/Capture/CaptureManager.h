//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#ifndef SENTRA_ENGINE_CAPTUREMANAGER_H
#define SENTRA_ENGINE_CAPTUREMANAGER_H

#include "Sentra/Network/InterfaceManager.h"

#include "CaptureSession.h"

namespace SCore {
    class CaptureManager {
    private:
        CaptureManager() :  m_InterfaceManager(InterfaceManager::Instance()) {}

    public:
        CaptureManager(const CaptureManager &) = delete;
        CaptureManager(CaptureManager&&) = delete;

        CaptureManager &operator=(const CaptureManager &) = delete;
        CaptureManager &operator=(CaptureManager &&) = delete;

        inline static CaptureManager& Instance() {
            static CaptureManager s_Instance;

            return s_Instance;
        }

    public:
        bool StartCaptureSession(const std::string& interfaceName);
        void StopCaptureSession(const std::string& interfaceName);

        void StopAllCaptureSessions();

        const CaptureSession* GetCaptureSession(const std::string& interfaceName) const;
        void GetAllCaptureSessions(std::vector<const CaptureSession*>& outSessions) const;

    private:
        InterfaceManager &m_InterfaceManager;

        std::unordered_map<std::string, std::unique_ptr<CaptureSession>> m_CaptureSessions{};
    };
}

#endif //SENTRA_ENGINE_CAPTUREMANAGER_H
