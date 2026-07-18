//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#include "Sentra/Capture/CaptureManager.h"

#include "Sentra/Core/Logger.h"
#include "Sentra/Utils/LoggerTags.h"

namespace SCore {
    bool CaptureManager::StartCaptureSession(const std::string& interfaceName) {
        if (const auto it = m_CaptureSessions.find(interfaceName); it != m_CaptureSessions.end()) {
            SCORE_TRACE("{}: Capture session already active on '{}'", LoggerTags::CAPTURE_ALREADY_ACTIVE, interfaceName);
            return true;
        }

        pcpp::PcapLiveDevice *interface = m_InterfaceManager.GetSystemInterface(interfaceName);

        if (interface == nullptr) {
            SCORE_TRACE("{}: Interface '{}' does not exist", LoggerTags::CAPTURE_NO_INTERFACE, interfaceName);
            return false;
        }

        auto session = std::make_unique<CaptureSession>(interface);

        if (!session->Start()) {
            SCORE_TRACE("{}: Capture initialization failed on interface '{}'", LoggerTags::CAPTURE_ERR, interfaceName);
            return false;
        }

        m_CaptureSessions[interfaceName] = std::move(session);
        SCORE_TRACE("{}: Interface '{}' capture state changed to up", LoggerTags::CAPTURE_STATE_CHANGE, interfaceName);

        return true;
    }

    void CaptureManager::StopCaptureSession(const std::string& interfaceName) {
        const auto it = m_CaptureSessions.find(interfaceName);

        if (it == m_CaptureSessions.end()) {
            SCORE_TRACE("{}: Capture session for interface '{}' is missing from the session database",
                LoggerTags::CAPTURE_ERR, interfaceName);
            return;
        }

        it->second->Stop();
        m_CaptureSessions.erase(it);

        SCORE_TRACE("{}: Interface '{}' capture state changed to down", LoggerTags::CAPTURE_STATE_CHANGE, interfaceName);
    }
}
