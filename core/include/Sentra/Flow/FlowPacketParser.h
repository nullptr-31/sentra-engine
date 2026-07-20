//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_FLOWPACKETPARSER_H
#define SENTRA_ENGINE_FLOWPACKETPARSER_H

#include <optional>

#include <Packet.h>
#include <RawPacket.h>

#include "FlowPacket.h"


namespace SCore {
    class FlowPacketParser {
    public:
        static std::optional<FlowPacket> TryParse(
            const pcpp::RawPacket& rawPacket,
            const pcpp::Packet& packet
        );
    };
} // SCore

#endif //SENTRA_ENGINE_FLOWPACKETPARSER_H
