//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_FLOWMANAGER_H
#define SENTRA_ENGINE_FLOWMANAGER_H

#include "Flow.h"
#include "FlowKey.h"
#include "FlowPacket.h"

namespace SCore {
    class FlowManager {
    public:
        void ProcessPacket(FlowPacket packet);

        std::size_t GetActiveFlowCount() const;

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
        std::unordered_map<FlowKey, TrackedFlow> m_Flows;
    };
} // SCore

#endif //SENTRA_ENGINE_FLOWMANAGER_H
