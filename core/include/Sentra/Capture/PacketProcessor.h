//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_PACKETPROCESSOR_H
#define SENTRA_ENGINE_PACKETPROCESSOR_H

#include <atomic>
#include <thread>

#include "RawPacket.h"

#include "BoundedPacketBuffer.h"
#include "Sentra/Flow/FlowManager.h"

namespace SCore {
    struct CaptureStats {
        std::atomic_uint64_t PacketsReceived = 0;
        std::atomic_uint64_t PacketsQueued = 0;
        std::atomic_uint64_t PacketsDropped = 0;
        std::atomic_uint64_t PacketsProcessed = 0;
        std::atomic_uint64_t ParseErrors = 0;

        void Reset() {
            PacketsReceived.store(0, std::memory_order_relaxed);
            PacketsQueued.store(0, std::memory_order_relaxed);
            PacketsDropped.store(0, std::memory_order_relaxed);
            PacketsProcessed.store(0, std::memory_order_relaxed);
            ParseErrors.store(0, std::memory_order_relaxed);
        }
    };

    class PacketProcessor {
    public:
        explicit PacketProcessor(BoundedPacketBuffer<pcpp::RawPacket>& buffer, CaptureStats& stats);
        ~PacketProcessor();

        void Start();
        void Stop();

        bool IsRunning() const ;

        std::uint64_t GetProcessedPacketCount() const;

    private:
        void Run(const std::stop_token& stopToken);

    private:
        FlowManager m_FlowManager;
        BoundedPacketBuffer<pcpp::RawPacket>& m_PacketBuffer;
        CaptureStats& m_Stats;

        std::jthread m_Worker;

        std::atomic_bool m_Running = false;
        std::atomic_uint64_t m_ProcessedPackets = 0;
    };
} // SCore

#endif //SENTRA_ENGINE_PACKETPROCESSOR_H
