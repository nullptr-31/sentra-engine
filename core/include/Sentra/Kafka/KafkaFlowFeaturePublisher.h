//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_KAFKAFLOWFEATUREPUBLISHER_H
#define SENTRA_ENGINE_KAFKAFLOWFEATUREPUBLISHER_H

#include <string>
#include <thread>

#include "Sentra/Capture/BoundedPacketBuffer.h"
#include "Sentra/Flow/FlowFeatures.h"

namespace SCore {
    class KafkaFlowFeaturePublisher {
    private:
        KafkaFlowFeaturePublisher() = default;

    public:
        KafkaFlowFeaturePublisher(const KafkaFlowFeaturePublisher &) = delete;

        KafkaFlowFeaturePublisher &operator=(const KafkaFlowFeaturePublisher &) = delete;

        static KafkaFlowFeaturePublisher &Instance() {
            static KafkaFlowFeaturePublisher instance;
            return instance;
        }

        bool Start();

        void Stop();

        bool Publish(FlowFeatures features);

    private:
        void Run(const std::stop_token &stopToken);

        static bool SerializeFeatures(const FlowFeatures &features, std::string &outJson);

    private:
        std::string m_Brokers = "localhost:9092";
        std::string m_TopicName = "sentra.flows.finished";

        BoundedPacketBuffer<FlowFeatures> m_Buffer{8192};

        std::jthread m_Worker;
        std::atomic_bool m_Running = false;
    };
} // SCore

#endif //SENTRA_ENGINE_KAFKAFLOWFEATUREPUBLISHER_H
