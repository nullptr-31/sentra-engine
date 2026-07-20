//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#include "Sentra/Capture/CaptureSession.h"

namespace SCore {
    CaptureSession::CaptureSession(pcpp::PcapLiveDevice *interface) : m_InterfaceManager(InterfaceManager::Instance()), m_Interface(interface) {
    }

    CaptureSession::~CaptureSession() {
        Stop();
    }

    bool CaptureSession::Start() {
        if (m_Interface == nullptr) return false;

        if (m_Capturing) return true;

        if (!m_Open) {
            if (!m_InterfaceManager.OpenInterface(m_Interface))
                return false;

            m_Open = true;
        }

        const bool started = m_Interface->startCapture(&CaptureSession::OnPacketArrives, this);

        if (!started) {
            m_InterfaceManager.CloseInterface(m_Interface);
            m_Open = false;

            return false;
        }

        m_Capturing = true;
        return true;
    }

    void CaptureSession::Stop() {
        if (m_Interface == nullptr) return;

        if (m_Capturing) {
            m_Interface->stopCapture();
            m_Capturing = false;
        }

        if (m_Open) {
            m_InterfaceManager.CloseInterface(m_Interface);
            m_Open = false;
        }
    }

    void CaptureSession::OnPacketArrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *device, void *cookie) {
        auto* session = static_cast<CaptureSession*>(cookie);

        if (session == nullptr || packet == nullptr) return;
    }
} // SCore