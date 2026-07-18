//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#ifndef SENTRA_ENGINE_CAPTURESESSION_H
#define SENTRA_ENGINE_CAPTURESESSION_H

#include <PcapLiveDevice.h>

namespace SCore {
    class CaptureSession {
    public:
        explicit CaptureSession(pcpp::PcapLiveDevice* interface);
        ~CaptureSession();

        bool Start();

        void Stop();
    private:
    };
} // SCore

#endif //SENTRA_ENGINE_CAPTURESESSION_H
