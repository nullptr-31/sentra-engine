//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#include "../../include/Sentra/Capture/CaptureSession.h"

namespace SCore {
    CaptureSession::CaptureSession(pcpp::PcapLiveDevice *interface) {
    }

    CaptureSession::~CaptureSession() {
        Stop();
    }

    bool CaptureSession::Start() {
    }

    void CaptureSession::Stop() {
    }
} // SCore