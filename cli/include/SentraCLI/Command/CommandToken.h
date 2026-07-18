//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#ifndef SENTRA_ENGINE_COMMANDTOKEN_H
#define SENTRA_ENGINE_COMMANDTOKEN_H

#include <string>

namespace SCLI {
    enum class CommandTokenType {
        Literal,
        Parameter,
    };

    struct CommandToken {
        CommandTokenType Type;
        std::string Value;
        std::string Description;

        static CommandToken Literal(std::string token, std::string desc = "") {
            return {CommandTokenType::Literal, std::move(token), std::move(desc)};
        }

        static CommandToken Parameter(std::string name,std::string desc = "") {
            return {CommandTokenType::Parameter, std::move(name), std::move(desc)};
        }
    };
}

#endif //SENTRA_ENGINE_COMMANDTOKEN_H
