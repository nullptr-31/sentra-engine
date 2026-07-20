//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#include <utility>

#include "Sentra/Flow/Flow.h"

namespace SCore {
    Flow::Flow(FlowKey key) : m_Key(std::move(key)) {
    }

    void Flow::AddPacket(const FlowPacket &packet) {
        if (m_FirstSeenUs == 0) {
            m_FirstSeenUs = packet.TimestampUs;
        }

        m_LastSeenUs = packet.TimestampUs;

        if (packet.Flags.Rst) {
            m_RstSeen = true;
            m_EndReason = FlowEndReason::TCPRst;
        }

        if (packet.Flags.Fin) {
            if (packet.Direction == FlowDirection::Forward) {
                m_ForwardFinSeen = true;
            } else if (packet.Direction == FlowDirection::Backward) {
                m_BackwardFinSeen = true;
            }

            if (m_ForwardFinSeen && m_BackwardFinSeen) {
                m_EndReason = FlowEndReason::TCPFin;
            }
        }

        m_Packets.emplace_back(packet);
    }

    bool Flow::HasEnded() const {
        return m_EndReason != FlowEndReason::None;
    }

    bool Flow::MarkExpired(const std::uint64_t currentTimestampUs, const std::uint64_t maxFlowDurationUs,
                           const std::uint64_t activityTimeoutUs) {
        if (m_EndReason != FlowEndReason::None) {
            return true;
        }

        if (m_FirstSeenUs != 0 && currentTimestampUs - m_FirstSeenUs > maxFlowDurationUs) {
            m_EndReason = FlowEndReason::MaxDuration;
            return true;
        }

        if (m_LastSeenUs != 0 && currentTimestampUs - m_LastSeenUs > activityTimeoutUs) {
            m_EndReason = FlowEndReason::ActivityTimeout;
            return true;
        }

        return false;
    }
} // SCore
