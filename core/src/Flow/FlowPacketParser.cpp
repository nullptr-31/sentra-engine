//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#include "Sentra/Flow/FlowPacketParser.h"

#include "IPv4Layer.h"
#include "IPv6Layer.h"
#include "SystemUtils.h"
#include "TcpLayer.h"
#include <UdpLayer.h>

namespace SCore {
    std::uint64_t ToEpochMicroseconds(const timespec &timestamp) {
        return static_cast<std::uint64_t>(timestamp.tv_sec) * 1000000ULL +
               static_cast<std::uint64_t>(timestamp.tv_nsec) / 1000ULL;
    }

    bool FillIPFields(const pcpp::Packet& packet, FlowPacket& flowPacket) {
        if (const auto* ipv4 = packet.getLayerOfType<pcpp::IPv4Layer>()) {
            flowPacket.SourceIP = ipv4->getSrcIPv4Address().toString();
            flowPacket.DestinationIP = ipv4->getDstIPv4Address().toString();
            flowPacket.Protocol = ipv4->getIPv4Header()->protocol;
            flowPacket.HeaderLength += static_cast<std::uint32_t>(ipv4->getHeaderLen());

            return true;
        }

        if (const auto* ipv6 = packet.getLayerOfType<pcpp::IPv6Layer>()) {
            flowPacket.SourceIP = ipv6->getSrcIPv6Address().toString();
            flowPacket.DestinationIP = ipv6->getDstIPv6Address().toString();
            flowPacket.Protocol = ipv6->getIPv6Header()->nextHeader;
            flowPacket.HeaderLength += static_cast<std::uint32_t>(ipv6->getHeaderLen());

            return true;
        }

        return false;
    }

    bool FillTCPFields(const pcpp::Packet& packet, FlowPacket& flowPacket) {
        const auto* tcp = packet.getLayerOfType<pcpp::TcpLayer>();

        if (tcp == nullptr) {
            return false;
        }

        const auto* header = tcp->getTcpHeader();

        flowPacket.SourcePort = tcp->getSrcPort();
        flowPacket.DestinationPort = tcp->getDstPort();
        flowPacket.Protocol = 6;

        flowPacket.Flags.Syn = header->synFlag;
        flowPacket.Flags.Ack = header->ackFlag;
        flowPacket.Flags.Fin = header->finFlag;
        flowPacket.Flags.Rst = header->rstFlag;
        flowPacket.Flags.Psh = header->pshFlag;
        flowPacket.Flags.Urg = header->urgFlag;
        flowPacket.Flags.Ece = header->eceFlag;
        flowPacket.Flags.Cwr = header->cwrFlag;

        flowPacket.WindowSize = pcpp::netToHost16(header->windowSize);
        flowPacket.SequenceNumber = pcpp::netToHost32(header->sequenceNumber);
        flowPacket.AcknowledgmentNumber = pcpp::netToHost32(header->ackNumber);

        flowPacket.HeaderLength += static_cast<std::uint32_t>(tcp->getHeaderLen());
        flowPacket.PayloadLength = static_cast<std::uint32_t>(tcp->getLayerPayloadSize());

        return true;
    }

    bool FillUDPFields(const pcpp::Packet& packet, FlowPacket& flowPacket) {
        const auto* udp = packet.getLayerOfType<pcpp::UdpLayer>();

        if (udp == nullptr) {
            return false;
        }

        flowPacket.SourcePort = udp->getSrcPort();
        flowPacket.DestinationPort = udp->getDstPort();
        flowPacket.Protocol = 17;

        flowPacket.HeaderLength += static_cast<std::uint32_t>(udp->getHeaderLen());
        flowPacket.PayloadLength = static_cast<std::uint32_t>(udp->getLayerPayloadSize());

        return true;
    }

    std::optional<FlowPacket> FlowPacketParser::TryParse(const pcpp::RawPacket &rawPacket, const pcpp::Packet &packet) {
        FlowPacket flowPacket;

        flowPacket.TimestampUs = ToEpochMicroseconds(rawPacket.getPacketTimeStamp());
        flowPacket.PacketLength = static_cast<std::uint32_t>(rawPacket.getRawDataLen());

        if (!FillIPFields(packet, flowPacket)) {
            return std::nullopt;
        }

        if (FillTCPFields(packet, flowPacket)) {
            return flowPacket;
        }

        if (FillUDPFields(packet, flowPacket)) {
            return flowPacket;
        }

        return std::nullopt;
    }
} // SCore
