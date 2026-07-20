//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#include <utility>

#include "../../include/Sentra/Flow/Flow.h"

namespace SCore {
    Flow::Flow(FlowKey key) : m_Key(std::move(key)) {
    }

    void Flow::AddPacket(const FlowPacket& packet) {
        if (packet.Direction == FlowDirection::Forward) {
            ++m_ForwardPacketCount;
            m_ForwardByteCount += packet.PacketLength;
        } else if (packet.Direction == FlowDirection::Backward) {
            ++m_BackwardPacketCount;
            m_BackwardByteCount += packet.PacketLength;
        }

        if (m_FirstSeenUs == 0) {
            m_FirstSeenUs = packet.TimestampUs;
        }

        m_LastSeenUs = packet.TimestampUs;

        m_Packets.emplace_back(packet);
    }
} // SCore