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
    enum class FlowEndReason {
        None = 0,
        MaxDuration,
        ActivityTimeout,
        TCPFin,
        TCPRst,
    };

    class Flow {
    public:
        explicit Flow(FlowKey key);

        void AddPacket(const FlowPacket &packet);

        bool HasEnded() const;

        bool MarkExpired(
            std::uint64_t currentTimestampUs,
            std::uint64_t maxFlowDurationUs,
            std::uint64_t activityTimeoutUs
        );

        const FlowKey &GetKey() const { return m_Key; }
        const std::vector<FlowPacket> &GetPackets() const { return m_Packets; }

        std::uint64_t GetFirstSeenUs() const { return m_FirstSeenUs; }
        std::uint64_t GetLastSeenUs() const { return m_LastSeenUs; }
        FlowEndReason GetEndReason() const { return m_EndReason; }

    private:
        FlowKey m_Key;

        // TODO(Abdelbaki) Update to a better suited storing solution
        // Current Risks;
        // m_Packets gets too big for long capture session
        std::vector<FlowPacket> m_Packets;

        std::uint64_t m_FirstSeenUs = 0;
        std::uint64_t m_LastSeenUs = 0;

        bool m_ForwardFinSeen = false;
        bool m_BackwardFinSeen = false;
        bool m_RstSeen = false;

        FlowEndReason m_EndReason = FlowEndReason::None;
    };
} // SCore

#endif //SENTRA_ENGINE_FLOW_H
