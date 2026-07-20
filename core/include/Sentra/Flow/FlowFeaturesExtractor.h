//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_FLOWFEATURESEXTRACTOR_H
#define SENTRA_ENGINE_FLOWFEATURESEXTRACTOR_H

#include <cstdint>

#include "Flow.h"
#include "FlowFeatures.h"

namespace SCore {
    class FlowFeatureExtractor {
    public:
        static FlowFeatures Extract(const Flow &flow, std::uint64_t activeIdleThresholdUs);
    };
} // SCore

#endif //SENTRA_ENGINE_FLOWFEATURESEXTRACTOR_H
