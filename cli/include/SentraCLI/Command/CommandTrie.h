//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#ifndef SENTRA_ENGINE_COMMANDTRIE_H
#define SENTRA_ENGINE_COMMANDTRIE_H

#include <functional>
#include <unordered_map>

#include "SentraCLI/CLI/CLIContext.h"
#include "SentraCLI/Command/CommandToken.h"

namespace SCLI {
    using CommandArgs = std::unordered_map<std::string, std::string>;
    using CommandHandler = std::function<void(CLIContext&, const CommandArgs&)>;

    struct CommandCandidate {
        std::string Value;
        std::string Description;
        bool IsParameter;
    };

    enum class CommandResolveStatus {
        Success,
        Unknown,
        Ambiguous,
        Incomplete,
    };

    struct CommandResolveResult {
        CommandResolveStatus Status;
        CommandHandler Handler;
        CommandArgs Arguments;

        std::vector<CommandCandidate> Candidates;
    };
    class CommandTrie {
    public:
        void Add(CLIMode mode, const std::vector<CommandToken>& tokens, const CommandHandler &handler);

        CommandResolveResult Resolve(CLIMode mode, const std::string &input) const;

        std::vector<CommandCandidate> GetHelpCandidates(CLIMode mode, const std::string& input) const;
        std::vector<std::string> GetCompletionCandidates(CLIMode mode, const std::string& input, int& replacementLength) const;


    private:
        struct Node {
            std::unordered_map<std::string, std::unique_ptr<Node>> Children;

            std::unique_ptr<Node> ParameterChild;
            std::string ParameterName;

            std::string Description;
            CommandHandler Handler;
        };

    private:
        std::vector<CommandCandidate> GetCandidates(const Node& node) const;
        std::vector<CommandCandidate> GetMatchingCandidates(const Node& node, const std::string& prefix) const;
        std::vector<std::string> GetLiteralCandidateTexts(const std::vector<CommandCandidate>& candidates) const;

    private:
        const Node* GetRoot(CLIMode mode) const;
    private:
        std::unordered_map<CLIMode, Node> m_Roots;
    };
} // SCLI

#endif //SENTRA_ENGINE_COMMANDTRIE_H
