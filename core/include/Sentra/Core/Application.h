//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#ifndef SENTRA_ENGINE_APPLICATION_H
#define SENTRA_ENGINE_APPLICATION_H

namespace SCore {
    class Application {
    public:
        Application();

        ~Application();

        void Run();

    private:
        bool m_IsRunning;
    };
}

#endif //SENTRA_ENGINE_APPLICATION_H
