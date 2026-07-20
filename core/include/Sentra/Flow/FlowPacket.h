//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_FLOWPACKET_H
#define SENTRA_ENGINE_FLOWPACKET_H

#include <string>

namespace SCore {
    enum class FlowDirection : std::uint8_t {
        Unknown = 0,
        Forward,
        Backward
    };

    struct TcpFlags {
        bool Syn = false;
        bool Ack = false;
        bool Fin = false;
        bool Rst = false;
        bool Psh = false;
        bool Urg = false;
        bool Ece = false;
        bool Cwr = false;
    };

    struct FlowPacket {
        std::string SourceIP;
        std::string DestinationIP;

        std::uint16_t SourcePort = 0;
        std::uint16_t DestinationPort = 0;

        std::uint8_t Protocol = 0;

        TcpFlags Flags;

        std::uint64_t TimestampUs = 0;

        std::uint32_t PacketLength = 0;
        std::uint32_t PayloadLength = 0;
        std::uint32_t HeaderLength = 0;

        std::uint16_t WindowSize = 0;

        std::uint32_t SequenceNumber = 0;
        std::uint32_t AcknowledgmentNumber = 0;

        FlowDirection Direction = FlowDirection::Unknown;
    };

}

#endif //SENTRA_ENGINE_FLOWPACKET_H
