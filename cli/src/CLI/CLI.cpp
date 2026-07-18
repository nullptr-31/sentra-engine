//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#include "SentraCLI/CLI/CLI.h"

#include <replxx.hxx>

namespace SCLI {
     CLI::CLI(const SCore::Application &application) : m_Application(application) {
    }

     void CLI::Run() const {
         replxx::Replxx rx;

         while (m_Application.IsRunning()) {
             const char *input = rx.input("sentra# ");

             if (input == nullptr) break;

             std::string line = input;
             if (line.empty()) continue;

             rx.history_add(line);
         }
    }
}