//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#ifndef SENTRA_ENGINE_STRINGUTILS_H
#define SENTRA_ENGINE_STRINGUTILS_H

#include <sstream>
#include <string>
#include <vector>

namespace SCLI {
    inline std::vector<std::string> Tokenize(const std::string& value) {
        std::vector<std::string> tokens;
        tokens.reserve(value.size());

        std::stringstream ss(value);
        std::string token;

        while (ss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    inline bool StartsWith(const std::string_view str, const std::string_view prefix) {
        if (prefix.size() > str.size()) return false;

        return str.compare(0, prefix.size(), prefix) == 0;
    }

    inline bool EndsWithSpace(const std::string_view str) {
        if (str.empty()) return false;

        return str.back() == ' ' || str.back() == '\t';
    }

}

#endif //SENTRA_ENGINE_STRINGUTILS_H
