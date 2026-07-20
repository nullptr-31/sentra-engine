//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#include "Sentra/Capture/PacketProcessor.h"

#include <Packet.h>

#include "Sentra/Flow/FlowPacketParser.h"

namespace SCore {
    PacketProcessor::PacketProcessor(BoundedPacketBuffer<pcpp::RawPacket> &buffer,
                                     CaptureStats &stats) : m_PacketBuffer(buffer), m_Stats(stats) {
    }

    PacketProcessor::~PacketProcessor() {
        Stop();
    }

    void PacketProcessor::Start() {
        if (bool expected = false; !m_Running.compare_exchange_strong(expected, true)) {
            return;
        }

        m_Worker = std::jthread([this](const std::stop_token &stopToken) {
            Run(stopToken);
        });
    }

    void PacketProcessor::Stop() {
        if (!m_Running.exchange(false) && !m_Worker.joinable()) {
            return;
        }

        m_PacketBuffer.Close();

        if (m_Worker.joinable()) {
            m_Worker.request_stop();
            m_Worker.join();
        }
    }

    bool PacketProcessor::IsRunning() const {
        return m_Running.load(std::memory_order_relaxed);
    }

    std::uint64_t PacketProcessor::GetProcessedPacketCount() const {
        return m_Stats.PacketsProcessed.load(std::memory_order_relaxed);
    }

    void PacketProcessor::Run(const std::stop_token &stopToken) {
        pcpp::RawPacket rawPacket;

        while (!stopToken.stop_requested()) {
            if (!m_PacketBuffer.WaitPop(rawPacket)) {
                break;
            }

            pcpp::Packet packet(&rawPacket);
            auto flowPacket = FlowPacketParser::TryParse(rawPacket, packet);

            if (!flowPacket.has_value()) {
                continue;
            }

            m_FlowManager.ProcessPacket(std::move(*flowPacket));
            m_Stats.PacketsProcessed.fetch_add(1, std::memory_order_relaxed);
        }

        m_Running.store(false, std::memory_order_relaxed);
    }
} // SCore
