//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#include "Sentra/Core/Application.h"

#include "Sentra/Core/Logger.h"
#include "Sentra/Utils/LoggerTags.h"

namespace SCore {
    Application::Application() : m_InterfaceManager(InterfaceManager::Instance()),
                                 m_CaptureManager(CaptureManager::Instance()),
                                 m_IsRunning(false) {
        SCORE_TRACE("{}: Application initialized", LoggerTags::UPDATE);
    }

    Application::~Application() {
        m_IsRunning = false;

        SCORE_TRACE("{}: Application shut down", LoggerTags::UPDATE);
    }

    void Application::Run() {
        if (m_IsRunning) return;

        m_IsRunning = true;

        SCORE_TRACE("{}: Application state changed to up", LoggerTags::STATE_CHANGE);
    }

    void Application::Stop() {
        if (!m_IsRunning) return;

        m_CaptureManager.StopAllCaptureSessions();

        m_IsRunning = false;

        SCORE_TRACE("{}: Application state changed to down", LoggerTags::STATE_CHANGE);
    }
}
