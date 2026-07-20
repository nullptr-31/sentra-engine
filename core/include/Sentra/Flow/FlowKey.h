//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_FLOWKEY_H
#define SENTRA_ENGINE_FLOWKEY_H

#include <cstdint>
#include <functional>
#include <string>

namespace SCore {
    enum class TransportProtocol : std::uint8_t {
        Unknown = 0,
        TCP,
        UDP,
        ICMP
    };

    struct FlowEndpoint {
        std::string IpAddress;
        std::uint16_t Port = 0;

        bool operator==(const FlowEndpoint& other) const {
            return IpAddress == other.IpAddress && Port == other.Port;
        }

        bool operator<(const FlowEndpoint& other) const {
            if (IpAddress != other.IpAddress) {
                return IpAddress < other.IpAddress;
            }

            return Port < other.Port;
        }
    };

    struct FlowKey {
        FlowEndpoint A;
        FlowEndpoint B;
        TransportProtocol Protocol = TransportProtocol::Unknown;

        bool operator==(const FlowKey& other) const {
            return A == other.A && B == other.B && Protocol == other.Protocol;
        }
    };

}

namespace std {
    template <>
    struct hash<SCore::FlowEndpoint> {
        std::size_t operator()(const SCore::FlowEndpoint& endpoint) const noexcept {
            const std::size_t ipHash = std::hash<std::string>{}(endpoint.IpAddress);
            const std::size_t portHash = std::hash<std::uint16_t>{}(endpoint.Port);

            return ipHash ^ (portHash << 1);
        }
    };

    template <>
    struct hash<SCore::FlowKey> {
        std::size_t operator()(const SCore::FlowKey& key) const noexcept {
            const std::size_t aHash = std::hash<SCore::FlowEndpoint>{}(key.A);
            const std::size_t bHash = std::hash<SCore::FlowEndpoint>{}(key.B);
            const std::size_t protocolHash = std::hash<std::uint8_t>{}(
                static_cast<std::uint8_t>(key.Protocol)
            );

            return aHash ^ (bHash << 1) ^ (protocolHash << 2);
        }
    };
}

#endif //SENTRA_ENGINE_FLOWKEY_H
