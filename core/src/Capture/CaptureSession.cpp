//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#include "Sentra/Capture/CaptureSession.h"

namespace SCore {
    CaptureSession::CaptureSession(pcpp::PcapLiveDevice *interface) : m_InterfaceManager(InterfaceManager::Instance()),
                                                                      m_Interface(interface),
                                                                      m_PacketBuffer(4096),
                                                                      m_PacketProcessor(m_PacketBuffer, m_Stats) {
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

        m_Stats.Reset();
        m_PacketBuffer.Reset();
        m_PacketProcessor.Start();

        const bool started = m_Interface->startCapture(&CaptureSession::OnPacketArrives, this);

        if (!started) {
            m_PacketProcessor.Stop();

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

        m_PacketProcessor.Stop();

        if (m_Open) {
            m_InterfaceManager.CloseInterface(m_Interface);
            m_Open = false;
        }
    }

    void CaptureSession::OnPacketArrives(const pcpp::RawPacket *packet, pcpp::PcapLiveDevice *device, void *cookie) {
        auto *session = static_cast<CaptureSession *>(cookie);

        if (session == nullptr || packet == nullptr) return;
        session->m_Stats.PacketsReceived.fetch_add(1, std::memory_order_relaxed);

        if (session->m_PacketBuffer.TryPush(*packet)) {
            session->m_Stats.PacketsQueued.fetch_add(1, std::memory_order_relaxed);
        } else {
            session->m_Stats.PacketsDropped.fetch_add(1, std::memory_order_relaxed);
        }
    }
} // SCore
