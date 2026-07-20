//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_FLOW_H
#define SENTRA_ENGINE_FLOW_H

#include <cstdint>
#include <string>

#include "FlowKey.h"
#include "FlowPacket.h"

namespace SCore {
    class Flow {
    public:
        explicit Flow(FlowKey  key);

        void AddPacket(const FlowPacket& packet);
    private:
        FlowKey m_Key;

        // TODO(Abdelbaki) Update to a better suited storing solution
        // Current Risks;
        // m_Packets gets too big for long capture session
        std::vector<FlowPacket> m_Packets;

        std::uint64_t m_ForwardPacketCount = 0;
        std::uint64_t m_BackwardPacketCount = 0;

        std::uint64_t m_ForwardByteCount = 0;
        std::uint64_t m_BackwardByteCount = 0;

        std::uint64_t m_FirstSeenUs = 0;
        std::uint64_t m_LastSeenUs = 0;
    };
} // SCore

#endif //SENTRA_ENGINE_FLOW_H
