//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#ifndef SENTRA_ENGINE_LOGGERTAGS_H
#define SENTRA_ENGINE_LOGGERTAGS_H
#include <string>

namespace SCore::LoggerTags {
    inline constexpr std::string_view UPDATE = "%SENTRA-UPDATE";
    inline constexpr std::string_view STATE_CHANGE = "%SENTRA-STATE_CHANGE";

    inline constexpr std::string_view CAPTURE_ERR = "%CAPTURE-ERR";
    inline constexpr std::string_view CAPTURE_NO_INTERFACE = "%CAPTURE-NOIF";
    inline constexpr std::string_view CAPTURE_ALREADY_ACTIVE = "%CAPTURE-ALREADY_ACTIVE";
    inline constexpr std::string_view CAPTURE_STATE_CHANGE = "%CAPTURE-STATE_CHANGE";


}

#endif //SENTRA_ENGINE_LOGGERTAGS_H
