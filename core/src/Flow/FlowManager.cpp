//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#include "Sentra/Flow/FlowManager.h"

#include <ranges>

namespace SCore {
    namespace {
        constexpr std::uint8_t TCP_PROTOCOL_NUMBER = 6;
        constexpr std::uint8_t UDP_PROTOCOL_NUMBER = 17;
        constexpr std::uint8_t ICMP_PROTOCOL_NUMBER = 1;
    }

    void FlowManager::ProcessPacket(FlowPacket packet) {
        FlowEndpoint source = MakeSourceEndpoint(packet);
        FlowEndpoint destination = MakeDestinationEndpoint(packet);
        const TransportProtocol protocol = GetTransportProtocol(packet);

        FlowKey key = MakeFlowKey(source, destination, protocol);

        std::lock_guard lock(m_Mutex);

        auto it = m_ActiveFlows.find(key);

        if (it == m_ActiveFlows.end()) {
            auto [newIt, inserted] = m_ActiveFlows.try_emplace(
                key,
                key,
                source,
                destination
            );

            packet.Direction = DetermineDirection(newIt->second, source, destination);
            newIt->second.FlowData.AddPacket(packet);
            return;
        }

        if (it->second.FlowData.IsExpired(
            packet.TimestampUs,
            m_MaxFlowDurationUs,
            m_ActivityTimeoutUs
        )) {
            TrackedFlow finishedFlow = std::move(it->second);
            m_ActiveFlows.erase(it);

            FinishFlow(std::move(finishedFlow));

            auto [newIt, inserted] = m_ActiveFlows.try_emplace(
                key,
                key,
                source,
                destination
            );

            packet.Direction = DetermineDirection(newIt->second, source, destination);
            newIt->second.FlowData.AddPacket(packet);
            return;
        }

        packet.Direction = DetermineDirection(it->second, source, destination);
        it->second.FlowData.AddPacket(packet);

        if (it->second.FlowData.HasEnded()) {
            TrackedFlow finishedFlow = std::move(it->second);
            m_ActiveFlows.erase(it);

            FinishFlow(std::move(finishedFlow));
        }
    }

    std::size_t FlowManager::GetActiveFlowCount() const {
        return m_ActiveFlows.size();
    }

    std::size_t FlowManager::GetFinishedFlowCount() const {
        return m_FinishedFlows.size();
    }

    std::vector<const Flow *> FlowManager::GetActiveFlows() const {
        return {};
    }

    std::vector<const Flow *> FlowManager::GetFinishedFlows() const {
        return {};
    }

    std::vector<const Flow *> FlowManager::GetAllFlows() const {
        return {};
    }

    void FlowManager::SetMaxFlowDuration(const std::uint64_t microseconds) {
        m_MaxFlowDurationUs = microseconds;
    }

    void FlowManager::SetActivityTimeout(const std::uint64_t microseconds) {
        m_ActivityTimeoutUs = microseconds;
    }

    void FlowManager::FinishFlow(TrackedFlow &&flow) {
        m_FinishedFlows.emplace_back(std::move(flow.FlowData));
    }

    void FlowManager::Clear() {
        m_ActiveFlows.clear();
        m_FinishedFlows.clear();
    }

    FlowManager::TrackedFlow::TrackedFlow(FlowKey key, FlowEndpoint forwardEndpoint,
                                          FlowEndpoint backwardEndpoint) : FlowData(std::move(key)),
                                                                           ForwardEndpoint(std::move(forwardEndpoint)),
                                                                           BackwardEndpoint(
                                                                               std::move(backwardEndpoint)) {
    }

    FlowEndpoint FlowManager::MakeSourceEndpoint(const FlowPacket &packet) {
        return FlowEndpoint{
            .IpAddress = packet.SourceIP,
            .Port = packet.SourcePort
        };
    }

    FlowEndpoint FlowManager::MakeDestinationEndpoint(const FlowPacket &packet) {
        return FlowEndpoint{
            .IpAddress = packet.DestinationIP,
            .Port = packet.DestinationPort
        };
    }

    TransportProtocol FlowManager::GetTransportProtocol(const FlowPacket &packet) {
        switch (packet.Protocol) {
            case TCP_PROTOCOL_NUMBER:
                return TransportProtocol::TCP;

            case UDP_PROTOCOL_NUMBER:
                return TransportProtocol::UDP;

            case ICMP_PROTOCOL_NUMBER:
                return TransportProtocol::ICMP;

            default:
                return TransportProtocol::Unknown;
        }
    }

    FlowKey FlowManager::MakeFlowKey(FlowEndpoint source, FlowEndpoint destination, TransportProtocol protocol) {
        if (destination < source) {
            std::swap(source, destination);
        }

        return FlowKey{
            .A = std::move(source),
            .B = std::move(destination),
            .Protocol = protocol
        };
    }

    FlowDirection FlowManager::DetermineDirection(const TrackedFlow &flow, const FlowEndpoint &source,
                                                  const FlowEndpoint &destination) {
        if (source == flow.ForwardEndpoint && destination == flow.BackwardEndpoint) {
            return FlowDirection::Forward;
        }

        if (source == flow.BackwardEndpoint && destination == flow.ForwardEndpoint) {
            return FlowDirection::Backward;
        }

        return FlowDirection::Unknown;
    }
} // SCore
