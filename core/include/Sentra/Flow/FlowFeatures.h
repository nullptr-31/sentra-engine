//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_FLOWFEATURES_H
#define SENTRA_ENGINE_FLOWFEATURES_H

#include <cstdint>

namespace SCore {
    struct FlowFeatures {
        std::uint16_t DestinationPort = 0;
        double FlowDuration = 0.0;

        std::uint32_t TotalFwdPackets = 0;
        std::uint32_t TotalBackwardPackets = 0;
        std::uint64_t TotalLengthOfFwdPackets = 0;
        std::uint64_t TotalLengthOfBwdPackets = 0;

        std::uint64_t FwdPacketLengthMax = 0;
        std::uint64_t FwdPacketLengthMin = 0;
        double FwdPacketLengthMean = 0.0;
        double FwdPacketLengthStd = 0.0;

        std::uint64_t BwdPacketLengthMax = 0;
        std::uint64_t BwdPacketLengthMin = 0;
        double BwdPacketLengthMean = 0.0;
        double BwdPacketLengthStd = 0.0;

        double FlowBytesPerSecond = 0.0;
        double FlowPacketsPerSecond = 0.0;

        double FlowIATMean = 0.0;
        double FlowIATStd = 0.0;
        double FlowIATMax = 0.0;
        double FlowIATMin = 0.0;

        double FwdIATTotal = 0.0;
        double FwdIATMean = 0.0;
        double FwdIATStd = 0.0;
        double FwdIATMax = 0.0;
        double FwdIATMin = 0.0;

        double BwdIATTotal = 0.0;
        double BwdIATMean = 0.0;
        double BwdIATStd = 0.0;
        double BwdIATMax = 0.0;
        double BwdIATMin = 0.0;

        std::uint32_t FwdPSHFlags = 0;
        std::uint32_t FwdURGFlags = 0;
        std::uint64_t FwdHeaderLength = 0;
        std::uint64_t BwdHeaderLength = 0;

        double FwdPacketsPerSecond = 0.0;
        double BwdPacketsPerSecond = 0.0;

        std::uint64_t MinPacketLength = 0;
        std::uint64_t MaxPacketLength = 0;
        double PacketLengthMean = 0.0;
        double PacketLengthStd = 0.0;
        double PacketLengthVariance = 0.0;

        std::uint32_t FINFlagCount = 0;
        std::uint32_t SYNFlagCount = 0;
        std::uint32_t RSTFlagCount = 0;
        std::uint32_t PSHFlagCount = 0;
        std::uint32_t ACKFlagCount = 0;
        std::uint32_t URGFlagCount = 0;
        std::uint32_t CWEFlagCount = 0;
        std::uint32_t ECEFlagCount = 0;

        double DownUpRatio = 0.0;
        double AveragePacketSize = 0.0;
        double AvgFwdSegmentSize = 0.0;
        double AvgBwdSegmentSize = 0.0;

        std::uint64_t FwdHeaderLengthDuplicated0 = 0;

        double SubflowFwdPackets = 0.0;
        double SubflowFwdBytes = 0.0;
        double SubflowBwdPackets = 0.0;
        double SubflowBwdBytes = 0.0;

        std::uint64_t InitWinBytesForward = 0;
        std::uint64_t InitWinBytesBackward = 0;
        std::uint32_t ActDataPktFwd = 0;
        std::uint64_t MinSegSizeForward = 0;

        double ActiveMean = 0.0;
        double ActiveStd = 0.0;
        double ActiveMax = 0.0;
        double ActiveMin = 0.0;

        double IdleMean = 0.0;
        double IdleStd = 0.0;
        double IdleMax = 0.0;
        double IdleMin = 0.0;
    };
}


#endif //SENTRA_ENGINE_FLOWFEATURES_H
