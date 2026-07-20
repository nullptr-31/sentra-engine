//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_FLOWMANAGER_H
#define SENTRA_ENGINE_FLOWMANAGER_H

#include <mutex>

#include "Flow.h"
#include "FlowKey.h"
#include "FlowPacket.h"

namespace SCore {
    class FlowManager {
    public:
        void ProcessPacket(FlowPacket packet);

        std::size_t GetActiveFlowCount() const;
        std::size_t GetFinishedFlowCount() const;

        std::vector<const Flow *> GetActiveFlows() const;
        std::vector<const Flow *> GetFinishedFlows() const;
        std::vector<const Flow *> GetAllFlows() const;

        void SetMaxFlowDuration(std::uint64_t microseconds);
        void SetActivityTimeout(std::uint64_t microseconds);
        void SetActiveIdleThreshold(std::uint64_t microseconds);

        void Clear();

    private:
        struct TrackedFlow {
            Flow FlowData;
            FlowEndpoint ForwardEndpoint;
            FlowEndpoint BackwardEndpoint;

            TrackedFlow(
                FlowKey key,
                FlowEndpoint forwardEndpoint,
                FlowEndpoint backwardEndpoint
            );
        };

    private:
        void FinishFlow(TrackedFlow &&flow);

    private:
        static FlowEndpoint MakeSourceEndpoint(const FlowPacket &packet);

        static FlowEndpoint MakeDestinationEndpoint(const FlowPacket &packet);

        static TransportProtocol GetTransportProtocol(const FlowPacket &packet);

        static FlowKey MakeFlowKey(
            FlowEndpoint source,
            FlowEndpoint destination,
            TransportProtocol protocol
        );

        static FlowDirection DetermineDirection(
            const TrackedFlow &flow,
            const FlowEndpoint &source,
            const FlowEndpoint &destination
        );

    private:
        mutable std::mutex m_Mutex;

        std::unordered_map<FlowKey, TrackedFlow> m_ActiveFlows;
        std::vector<Flow> m_FinishedFlows;

        std::uint64_t m_MaxFlowDurationUs = 3600ULL * 1000000ULL;
        std::uint64_t m_ActivityTimeoutUs = 300ULL * 1000000ULL;
        std::uint64_t m_ActiveIdleThresholdUs = 5ULL * 1000000ULL;
    };
} // SCore

#endif //SENTRA_ENGINE_FLOWMANAGER_H
