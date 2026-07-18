//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#include "../../include/SentraCLI/Command/CommandTrie.h"

#include "SentraCLI/Utils/StringUtils.h"

namespace SCLI {
    void CommandTrie::Add(CLIMode mode, const std::vector<CommandToken> &tokens, const CommandHandler &handler) {
        Node *current = &m_Roots[mode];

        for (const auto &[Type, Value, Description] : tokens) {
            if (Type == CommandTokenType::Literal) {
                auto& child = current->Children[Value];

                if (!child)
                    child = std::make_unique<Node>();

                child->Description = Description;
                current = child.get();
            } else {
                if (!current->ParameterChild)
                    current->ParameterChild = std::make_unique<Node>();

                current->ParameterChild->ParameterName = Value;
                current->ParameterChild->Description = Description;

                current = current->ParameterChild.get();
            }
        }

        current->Handler = handler;
    }

    CommandResolveResult CommandTrie::Resolve(const CLIMode mode, const std::string &input) const {
        CommandResolveResult result;
        result.Status = CommandResolveStatus::Unknown;

        const Node* current = GetRoot(mode);
        if (!current) return result;

        const auto inputTokens = Tokenize(input);

        if (inputTokens.empty()) {
            result.Status = CommandResolveStatus::Incomplete;
            result.Candidates = GetCandidates(*current);

            return result;
        }

        for (const auto& token : inputTokens) {
            if (auto exactIt = current->Children.find(token); exactIt != current->Children.end()) {
                current = exactIt->second.get();
                continue;
            }

            std::vector<const Node *> literalMatches;
            std::vector<CommandCandidate> literalCandidates;

            for (const auto &[fst, snd]: current->Children) {
                if (const std::string &commandToken = fst; StartsWith(commandToken, token)) {
                    literalMatches.push_back(snd.get());
                    literalCandidates.push_back({
                        commandToken,
                        snd->Description,
                        false,
                    });
                }
            }

            if (literalMatches.size() > 1) {
                result.Status = CommandResolveStatus::Ambiguous;
                result.Candidates = literalCandidates;

                return result;
            }

            if (literalMatches.size() == 1) {
                current = literalMatches[0];
                continue;
            }

            if (current->ParameterChild) {
                result.Arguments[current->ParameterChild->ParameterName] = token;
                current = current->ParameterChild.get();
                continue;
            }

            result.Status = CommandResolveStatus::Unknown;
            result.Candidates = GetCandidates(*current);
            return result;
        }

        if (current->Handler) {
            result.Status = CommandResolveStatus::Success;
            result.Handler = current->Handler;
            return result;
        }

        result.Status = CommandResolveStatus::Incomplete;
        result.Candidates = GetCandidates(*current);

        return result;
    }

    std::vector<CommandCandidate> CommandTrie::GetHelpCandidates(const CLIMode mode, const std::string &input) const {
        const Node *current = GetRoot(mode);

        if (!current)
            return {};

        const std::vector<std::string> tokens = Tokenize(input);

        if (tokens.empty())
            return GetCandidates(*current);

        const bool wantsNextToken = EndsWithSpace(input);

        size_t tokensToWalk = tokens.size();

        if (!wantsNextToken)
            tokensToWalk = tokens.size() - 1;

        for (size_t i = 0; i < tokensToWalk; ++i) {
            const std::string &inputToken = tokens[i];

            if (auto exactIt = current->Children.find(inputToken); exactIt != current->Children.end()) {
                current = exactIt->second.get();
                continue;
            }

            std::vector<const Node *> literalMatches;

            for (const auto &[fst, snd]: current->Children) {
                if (StartsWith(fst, inputToken))
                    literalMatches.push_back(snd.get());
            }

            if (literalMatches.size() == 1) {
                current = literalMatches[0];
                continue;
            }

            if (literalMatches.size() > 1)
                return {};

            if (current->ParameterChild) {
                current = current->ParameterChild.get();
                continue;
            }

            return {};
        }

        if (wantsNextToken)
            return GetCandidates(*current);

        const std::string &partialToken = tokens.back();

        return GetMatchingCandidates(*current, partialToken);
    }

    std::vector<std::string> CommandTrie::GetCompletionCandidates(const CLIMode mode, const std::string &input,
        int &replacementLength) const {
        replacementLength = 0;

        const Node *current = GetRoot(mode);

        if (!current)
            return {};

        const std::vector<std::string> tokens = Tokenize(input);

        if (tokens.empty())
            return GetLiteralCandidateTexts(GetCandidates(*current));

        bool wantsNextToken = EndsWithSpace(input);

        size_t tokensToWalk = tokens.size();

        if (!wantsNextToken) {
            replacementLength = static_cast<int>(tokens.back().size());
            tokensToWalk = tokens.size() - 1;
        }

        for (size_t i = 0; i < tokensToWalk; ++i) {
            const std::string &inputToken = tokens[i];

            if (auto exactIt = current->Children.find(inputToken); exactIt != current->Children.end()) {
                current = exactIt->second.get();
                continue;
            }

            std::vector<const Node *> literalMatches;

            for (const auto &[fst, snd]: current->Children) {
                if (StartsWith(fst, inputToken))
                    literalMatches.push_back(snd.get());
            }

            if (literalMatches.size() == 1) {
                current = literalMatches[0];
                continue;
            }

            if (literalMatches.size() > 1)
                return {};

            if (current->ParameterChild) {
                current = current->ParameterChild.get();
                continue;
            }

            return {};
        }

        if (wantsNextToken)
            return GetLiteralCandidateTexts(GetCandidates(*current));

        return GetLiteralCandidateTexts(
            GetMatchingCandidates(*current, tokens.back()));
    }

    std::vector<CommandCandidate> CommandTrie::GetCandidates(const Node &node) const {
        std::vector<CommandCandidate> candidates;
        candidates.reserve(node.Children.size());

        for (const auto&[fst, snd] : node.Children) {
            candidates.push_back({fst, snd->Description, false});
        }

        if (node.ParameterChild) {
            candidates.push_back({"<" + node.ParameterChild->ParameterName + ">",
                node.ParameterChild->Description,
                true});
        }

        return candidates;
    }

    std::vector<CommandCandidate> CommandTrie::GetMatchingCandidates(const Node &node, const std::string &prefix) const {
        if (const auto exactIt = node.Children.find(prefix); exactIt != node.Children.end()) {
            return {
                CommandCandidate{
                    exactIt->first,
                    exactIt->second->Description,
                    false
                }
            };
        }


        std::vector<CommandCandidate> candidates;

        for (const auto &[fst, snd]: node.Children) {
            if (StartsWith(fst, prefix)) {
                candidates.push_back(
                    {fst, snd->Description, false});
            }
        }

        if (node.ParameterChild) {
            candidates.push_back({
                "<" + node.ParameterChild->ParameterName + ">",
                node.ParameterChild->Description, true
            });
        }

        return candidates;
    }

    std::vector<std::string> CommandTrie::GetLiteralCandidateTexts(const std::vector<CommandCandidate> &candidates) const {
        std::vector<std::string> texts;

        for (const CommandCandidate &candidate: candidates) {
            if (!candidate.IsParameter)
                texts.push_back(candidate.Value);
        }

        return texts;
    }

    const CommandTrie::Node * CommandTrie::GetRoot(CLIMode mode) const {
        const auto it = m_Roots.find(mode);

        if (it == m_Roots.end())
            return nullptr;

        return &it->second;
    }
} // SCLI