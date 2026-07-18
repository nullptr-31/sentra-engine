//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#include <Sentra/Core/Application.h>
#include "Sentra/Core/Logger.h"

#include <SentraCLI/CLI/CLI.h>


int main() {
    SCore::Logger::Init();

    SCore::Application app;
    const SCLI::CLI cli(app);

    app.Run();

    cli.Run();

    app.Stop();

    return 0;
}
