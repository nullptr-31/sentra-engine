//
// Created by Abdelbaki Boukerche on 17/7/2026.
//

#include "SentraCLI/CLI/CLI.h"

#include <iomanip>
#include <iostream>
#include <replxx.hxx>

namespace SCLI {
     CLI::CLI(const SCore::Application &application) : m_Application(application) {
         m_Context = std::make_unique<CLIContext>();
         m_CommandTrie = std::make_unique<CommandTrie>();

         RegisterCommands();
    }

     void CLI::Run() const {
         replxx::Replxx rx;

         rx.set_completion_callback(
            [this](const std::string &context, int &replacementLength) {
                replxx::Replxx::completions_t completions;

                const std::vector<std::string> candidates =
                        m_CommandTrie->GetCompletionCandidates(
                            m_Context->GetCLIMode(), context, replacementLength);

                for (const std::string &candidate: candidates)
                    completions.emplace_back(candidate);

                return completions;
            });

         while (m_Application.IsRunning() && !m_Context->QuitRequested()) {
             const char *input = rx.input(m_Context->GetPrompt());

             if (input == nullptr) break;

             std::string line = input;
             if (line.empty()) continue;

             rx.history_add(line);

             HandleLine(line);
         }
    }

     void CLI::RegisterCommands() const {
         RegisterExecCommands();
         RegisterConfigCommands();
         RegisterInterfaceConfigCommands();
     }

     void CLI::RegisterExecCommands() const {
         constexpr auto mode = CLIMode::Exec;

         m_CommandTrie->Add(mode,
                           {
                               CommandToken::Literal("exit"),
                           },
                           [](CLIContext &ctx, const CommandArgs &) { ctx.RequestQuit(); });

         m_CommandTrie->Add(mode, {
                               CommandToken::Literal("configure"),
                               CommandToken::Literal("terminal"),
                           },
                           [](CLIContext &ctx, const CommandArgs &) {
                               ctx.SetCLIMode(CLIMode::Config);
                           });
     }

     void CLI::RegisterConfigCommands() const {
         constexpr auto mode = CLIMode::Config;

         m_CommandTrie->Add(mode, {
                               CommandToken::Literal("exit"),
                           }, [](CLIContext &ctx, const CommandArgs &) {
                               ctx.SetCLIMode(CLIMode::Exec);
                           });

         m_CommandTrie->Add(mode, {
                                CommandToken::Literal("end"),
                            }, [](CLIContext &ctx, const CommandArgs &) {
                                ctx.SetCLIMode(CLIMode::Exec);
                            });

         m_CommandTrie->Add(mode, {
                               CommandToken::Literal("interface"),
                               CommandToken::Parameter("name"),
                           }, [](CLIContext &ctx, const CommandArgs &args) {
                               ctx.SetCLIMode(CLIMode::InterfaceConfig);
                               ctx.SetCurrentInterface(args.at("name"));
                           });
     }

     void CLI::RegisterInterfaceConfigCommands() const {
         constexpr auto mode = CLIMode::InterfaceConfig;

         m_CommandTrie->Add(mode, {
                               CommandToken::Literal("exit"),
                           }, [](CLIContext &ctx, const CommandArgs &) {
                               ctx.SetCLIMode(CLIMode::Config);
                           });

         m_CommandTrie->Add(mode, {
                                CommandToken::Literal("end"),
                            }, [](CLIContext &ctx, const CommandArgs &) {
                                ctx.SetCLIMode(CLIMode::Exec);
                            });
     }

     void CLI::HandleLine(const std::string& line) const {
         if (const int questionMarkPos = static_cast<int>(line.find('?')); questionMarkPos != std::string::npos) {
             std::string helpInput = line;
             helpInput.erase(questionMarkPos, 1);

             const std::vector<CommandCandidate> candidates =
                     m_CommandTrie->GetHelpCandidates(m_Context->GetCLIMode(), helpInput);

             PrintCandidates(candidates);
             return;
         }

         auto [Status, Handler, Arguments, Candidates] =
                 m_CommandTrie->Resolve(m_Context->GetCLIMode(), line);

         switch (Status) {
             case CommandResolveStatus::Success:
                 Handler(*m_Context, Arguments);
                 break;
             case CommandResolveStatus::Unknown:
                 std::cout << "Unknown command" << std::endl;
                 PrintCandidates(Candidates);
                 break;
             case CommandResolveStatus::Ambiguous:
                 std::cout << "Ambiguous command" << std::endl;
                 PrintCandidates(Candidates);
                 break;
             case CommandResolveStatus::Incomplete:
                 std::cout << "Incomplete command" << std::endl;
                 PrintCandidates(Candidates);
                 break;
         }
     }

     void CLI::PrintCandidates(const std::vector<CommandCandidate> &candidates) const {
         if (candidates.empty()) {
             std::cout << "No completions available" << std::endl;
             return;
         }

         std::cout << "Possible completions:" << std::endl;

         for (const CommandCandidate &candidate: candidates) {
             std::cout << "  " << std::left << std::setw(16) << candidate.Value;

             if (!candidate.Description.empty())
                 std::cout << candidate.Description;

             std::cout << std::endl;
         }
     }
}
