//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_FLOWFEATURESSERIALIZER_H
#define SENTRA_ENGINE_FLOWFEATURESSERIALIZER_H

#include "Sentra/Flow/FlowFeatures.h"

namespace SCore {
    struct FlowFeaturesJSON {
        std::uint16_t destination_port = 0;
        double flow_duration = 0.0;

        std::uint32_t total_fwd_packets = 0;
        std::uint32_t total_backward_packets = 0;
        std::uint64_t total_length_of_fwd_packets = 0;
        std::uint64_t total_length_of_bwd_packets = 0;

        std::uint64_t fwd_packet_length_max = 0;
        std::uint64_t fwd_packet_length_min = 0;
        double fwd_packet_length_mean = 0.0;
        double fwd_packet_length_std = 0.0;

        std::uint64_t bwd_packet_length_max = 0;
        std::uint64_t bwd_packet_length_min = 0;
        double bwd_packet_length_mean = 0.0;
        double bwd_packet_length_std = 0.0;

        double flow_bytes_per_second = 0.0;
        double flow_packets_per_second = 0.0;

        double flow_iat_mean = 0.0;
        double flow_iat_std = 0.0;
        double flow_iat_max = 0.0;
        double flow_iat_min = 0.0;

        double fwd_iat_total = 0.0;
        double fwd_iat_mean = 0.0;
        double fwd_iat_std = 0.0;
        double fwd_iat_max = 0.0;
        double fwd_iat_min = 0.0;

        double bwd_iat_total = 0.0;
        double bwd_iat_mean = 0.0;
        double bwd_iat_std = 0.0;
        double bwd_iat_max = 0.0;
        double bwd_iat_min = 0.0;

        std::uint32_t fwd_psh_flags = 0;
        std::uint32_t fwd_urg_flags = 0;
        std::uint64_t fwd_header_length = 0;
        std::uint64_t bwd_header_length = 0;

        double fwd_packets_per_second = 0.0;
        double bwd_packets_per_second = 0.0;

        std::uint64_t min_packet_length = 0;
        std::uint64_t max_packet_length = 0;
        double packet_length_mean = 0.0;
        double packet_length_std = 0.0;
        double packet_length_variance = 0.0;

        std::uint32_t fin_flag_count = 0;
        std::uint32_t syn_flag_count = 0;
        std::uint32_t rst_flag_count = 0;
        std::uint32_t psh_flag_count = 0;
        std::uint32_t ack_flag_count = 0;
        std::uint32_t urg_flag_count = 0;
        std::uint32_t cwe_flag_count = 0;
        std::uint32_t ece_flag_count = 0;

        double down_up_ratio = 0.0;
        double average_packet_size = 0.0;
        double avg_fwd_segment_size = 0.0;
        double avg_bwd_segment_size = 0.0;

        std::uint64_t fwd_header_length_duplicated_0 = 0;

        double subflow_fwd_packets = 0.0;
        double subflow_fwd_bytes = 0.0;
        double subflow_bwd_packets = 0.0;
        double subflow_bwd_bytes = 0.0;

        std::uint64_t init_win_bytes_forward = 0;
        std::uint64_t init_win_bytes_backward = 0;
        std::uint32_t act_data_pkt_fwd = 0;
        std::uint64_t min_seg_size_forward = 0;

        double active_mean = 0.0;
        double active_std = 0.0;
        double active_max = 0.0;
        double active_min = 0.0;

        double idle_mean = 0.0;
        double idle_std = 0.0;
        double idle_max = 0.0;
        double idle_min = 0.0;
    };

    inline FlowFeaturesJSON ToFlowFeaturesJSON(const FlowFeatures &f) {
        return {
            .destination_port = f.DestinationPort,
            .flow_duration = f.FlowDuration,
            .total_fwd_packets = f.TotalFwdPackets,
            .total_backward_packets = f.TotalBackwardPackets,
            .total_length_of_fwd_packets = f.TotalLengthOfFwdPackets,
            .total_length_of_bwd_packets = f.TotalLengthOfBwdPackets,
            .fwd_packet_length_max = f.FwdPacketLengthMax,
            .fwd_packet_length_min = f.FwdPacketLengthMin,
            .fwd_packet_length_mean = f.FwdPacketLengthMean,
            .fwd_packet_length_std = f.FwdPacketLengthStd,
            .bwd_packet_length_max = f.BwdPacketLengthMax,
            .bwd_packet_length_min = f.BwdPacketLengthMin,
            .bwd_packet_length_mean = f.BwdPacketLengthMean,
            .bwd_packet_length_std = f.BwdPacketLengthStd,
            .flow_bytes_per_second = f.FlowBytesPerSecond,
            .flow_packets_per_second = f.FlowPacketsPerSecond,
            .flow_iat_mean = f.FlowIATMean,
            .flow_iat_std = f.FlowIATStd,
            .flow_iat_max = f.FlowIATMax,
            .flow_iat_min = f.FlowIATMin,
            .fwd_iat_total = f.FwdIATTotal,
            .fwd_iat_mean = f.FwdIATMean,
            .fwd_iat_std = f.FwdIATStd,
            .fwd_iat_max = f.FwdIATMax,
            .fwd_iat_min = f.FwdIATMin,
            .bwd_iat_total = f.BwdIATTotal,
            .bwd_iat_mean = f.BwdIATMean,
            .bwd_iat_std = f.BwdIATStd,
            .bwd_iat_max = f.BwdIATMax,
            .bwd_iat_min = f.BwdIATMin,
            .fwd_psh_flags = f.FwdPSHFlags,
            .fwd_urg_flags = f.FwdURGFlags,
            .fwd_header_length = f.FwdHeaderLength,
            .bwd_header_length = f.BwdHeaderLength,
            .fwd_packets_per_second = f.FwdPacketsPerSecond,
            .bwd_packets_per_second = f.BwdPacketsPerSecond,
            .min_packet_length = f.MinPacketLength,
            .max_packet_length = f.MaxPacketLength,
            .packet_length_mean = f.PacketLengthMean,
            .packet_length_std = f.PacketLengthStd,
            .packet_length_variance = f.PacketLengthVariance,
            .fin_flag_count = f.FINFlagCount,
            .syn_flag_count = f.SYNFlagCount,
            .rst_flag_count = f.RSTFlagCount,
            .psh_flag_count = f.PSHFlagCount,
            .ack_flag_count = f.ACKFlagCount,
            .urg_flag_count = f.URGFlagCount,
            .cwe_flag_count = f.CWEFlagCount,
            .ece_flag_count = f.ECEFlagCount,
            .down_up_ratio = f.DownUpRatio,
            .average_packet_size = f.AveragePacketSize,
            .avg_fwd_segment_size = f.AvgFwdSegmentSize,
            .avg_bwd_segment_size = f.AvgBwdSegmentSize,
            .fwd_header_length_duplicated_0 = f.FwdHeaderLengthDuplicated0,
            .subflow_fwd_packets = f.SubflowFwdPackets,
            .subflow_fwd_bytes = f.SubflowFwdBytes,
            .subflow_bwd_packets = f.SubflowBwdPackets,
            .subflow_bwd_bytes = f.SubflowBwdBytes,
            .init_win_bytes_forward = f.InitWinBytesForward,
            .init_win_bytes_backward = f.InitWinBytesBackward,
            .act_data_pkt_fwd = f.ActDataPktFwd,
            .min_seg_size_forward = f.MinSegSizeForward,
            .active_mean = f.ActiveMean,
            .active_std = f.ActiveStd,
            .active_max = f.ActiveMax,
            .active_min = f.ActiveMin,
            .idle_mean = f.IdleMean,
            .idle_std = f.IdleStd,
            .idle_max = f.IdleMax,
            .idle_min = f.IdleMin,
        };
    }
}

#endif //SENTRA_ENGINE_FLOWFEATURESSERIALIZER_H
