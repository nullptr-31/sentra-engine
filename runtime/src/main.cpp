//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#include <Sentra/Core/Application.h>
#include "Sentra/Core/Logger.h"

#include <SentraCLI/CLI/CLI.h>


int main() {
    SCore::Logger::Init();

    SCore::Application app;
    SCLI::CLI cli;

    app.Run();

    return 0;
}
