//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#include "Sentra/Kafka/KafkaFlowFeaturePublisher.h"

#include "rdkafkacpp.h"
#include <glaze/glaze.hpp>

#include "Sentra/Core/Logger.h"
#include "Sentra/Flow/FlowFeaturesSerializer.h"

namespace SCore {
    namespace {
        class DeliveryReportCallback final : public RdKafka::DeliveryReportCb {
        public:
            void dr_cb(RdKafka::Message &message) override {
                if (message.err()) {
                    S_ERROR("%SENTRA-3-KAFKA_DELIVERY_FAILED: {}\n", message.errstr());
                }
            }
        };

        void SetConfig(
            RdKafka::Conf &config,
            const std::string &key,
            const std::string &value
        ) {
            if (std::string error; config.set(key, value, error) != RdKafka::Conf::CONF_OK) {
                throw std::runtime_error(error);
            }
        }
    }

    bool KafkaFlowFeaturePublisher::Start() {
        if (bool expected = false; !m_Running.compare_exchange_strong(expected, true)) {
            return true;
        }

        m_Buffer.Reset();

        m_Worker = std::jthread([this](const std::stop_token &stopToken) {
            Run(stopToken);
        });

        return true;
    }

    void KafkaFlowFeaturePublisher::Stop() {
        if (!m_Running.exchange(false) && !m_Worker.joinable()) {
            return;
        }

        m_Buffer.Close();

        if (m_Worker.joinable()) {
            m_Worker.request_stop();
            m_Worker.join();
        }
    }

    bool KafkaFlowFeaturePublisher::Publish(FlowFeatures features) {
        return m_Buffer.TryPush(std::move(features));
    }

    void KafkaFlowFeaturePublisher::Run(const std::stop_token &stopToken) {
        DeliveryReportCallback deliveryCallback;

        std::string error;

        std::unique_ptr<RdKafka::Conf> config(
            RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)
        );

        SetConfig(*config, "bootstrap.servers", m_Brokers);
        SetConfig(*config, "client.id", "sentra-engine");
        SetConfig(*config, "acks", "1");
        SetConfig(*config, "compression.type", "snappy");
        SetConfig(*config, "linger.ms", "10");
        SetConfig(*config, "batch.num.messages", "1000");

        if (config->set("dr_cb", &deliveryCallback, error) != RdKafka::Conf::CONF_OK) {
            throw std::runtime_error(error);
        }

        std::unique_ptr<RdKafka::Producer> producer(
            RdKafka::Producer::create(config.get(), error)
        );

        if (!producer) {
            throw std::runtime_error(error);
        }

        FlowFeatures features;

        std::string payload;
        while (!stopToken.stop_requested()) {
            if (!m_Buffer.WaitPop(features)) {
                break;
            }

            if (!SerializeFeatures(features, payload)) {
                continue;
            }

            const RdKafka::ErrorCode result = producer->produce(
                m_TopicName,
                RdKafka::Topic::PARTITION_UA,
                RdKafka::Producer::RK_MSG_COPY,
                const_cast<char *>(payload.data()),
                payload.size(),
                nullptr,
                0,
                0,
                nullptr,
                nullptr
            );

            if (result != RdKafka::ERR_NO_ERROR) {
                S_ERROR("%SENTRA-3-KAFKA_PRODUCE_FAILED: {}\n", RdKafka::err2str(result));
            }

            producer->poll(0);
        }

        producer->flush(5000);
        m_Running.store(false, std::memory_order_relaxed);
    }

    bool KafkaFlowFeaturePublisher::SerializeFeatures(const FlowFeatures &features, std::string &outJson) {
        outJson.clear();

        FlowFeaturesJSON json = ToFlowFeaturesJSON(features);

        if (const auto error = glz::write_json(json, outJson)) {
            // TODO(add values): route Glaze serialization errors through Sentra logger.

            S_ERROR("%SENTRA-3-FLOW_SERIALIZE_FAILED: Glaze JSON serialization failed\n");
            return false;
        }

        return true;
    }
} // SCore
