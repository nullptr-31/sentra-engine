//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#include "Sentra/Core/Application.h"

#include "Sentra/Core/Logger.h"

namespace SCore {
    Application::Application() : m_IsRunning(false) {
        SCORE_TRACE("%SENTRA-INIT: Application initialized");
    }

    Application::~Application() {
        m_IsRunning = false;

        SCORE_TRACE("%SENTRA-STATE_CHANGE: Application state changed to down");
    }

    void Application::Run() {
        if (m_IsRunning) return;

        m_IsRunning = true;

        SCORE_TRACE("%SENTRA-STATE_CHANGE: Application state changed to up");
    }
}
