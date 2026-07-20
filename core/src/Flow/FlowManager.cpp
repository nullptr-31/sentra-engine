//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#include "Sentra/Flow/FlowManager.h"

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

        auto [it, inserted] = m_Flows.try_emplace(
            key,
            key,
            source,
            destination
        );

        packet.Direction = DetermineDirection(
            it->second,
            source,
            destination
        );

        it->second.FlowData.AddPacket(packet);
    }

    std::size_t FlowManager::GetActiveFlowCount() const {
        return m_Flows.size();
    }

    void FlowManager::Clear() {
        m_Flows.clear();
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
