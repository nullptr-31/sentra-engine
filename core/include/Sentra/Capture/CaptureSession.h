//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#ifndef SENTRA_ENGINE_CAPTURESESSION_H
#define SENTRA_ENGINE_CAPTURESESSION_H

#include <PcapLiveDevice.h>

#include "Sentra/Network/InterfaceManager.h"

namespace SCore {
    class CaptureSession {
    public:
        explicit CaptureSession(pcpp::PcapLiveDevice* interface);
        ~CaptureSession();

        bool Start();

        void Stop();

        inline bool IsOpen() const { return m_Open; }
        inline bool IsCapturing() const { return m_Capturing; }
        inline const pcpp::PcapLiveDevice *GetInterface() const { return m_Interface; }

    private:
        static void OnPacketArrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *device, void *cookie);

    private:
        InterfaceManager &m_InterfaceManager;

        pcpp::PcapLiveDevice* m_Interface;

        bool m_Open = false;
        bool m_Capturing = false;
    };
} // SCore

#endif //SENTRA_ENGINE_CAPTURESESSION_H
