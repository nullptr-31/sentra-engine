//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#include "Sentra/Flow/FlowFeaturesExtractor.h"

#include <cmath>
#include <numeric>

namespace SCore {
    namespace {
        constexpr double MicrosecondsPerSecond = 1000000.0;
        constexpr double SubflowThresholdUs = 1000000.0;

        double Divide(const double a, const double b) {
            return b == 0.0 ? 0.0 : a / b;
        }

        double Sum(const std::vector<double> &values) {
            return std::accumulate(values.begin(), values.end(), 0.0);
        }

        double Mean(const std::vector<double> &values) {
            return values.empty() ? 0.0 : Sum(values) / static_cast<double>(values.size());
        }

        double Variance(const std::vector<double> &values) {
            if (values.size() < 2) return 0.0;

            const double mean = Mean(values);
            double total = 0.0;

            for (const double value : values) {
                const double diff = value - mean;
                total += diff * diff;
            }

            return total / static_cast<double>(values.size() - 1);
        }

        double Std(const std::vector<double> &values) {
            return std::sqrt(Variance(values));
        }

        double Min(const std::vector<double> &values) {
            return values.empty() ? 0.0 : *std::min_element(values.begin(), values.end());
        }

        double Max(const std::vector<double> &values) {
            return values.empty() ? 0.0 : *std::max_element(values.begin(), values.end());
        }
    }

    FlowFeatures FlowFeatureExtractor::Extract(const Flow &flow, const std::uint64_t activeIdleThresholdUs) {
        FlowFeatures f;
        const auto &packets = flow.GetPackets();

        if (packets.empty()) {
            return f;
        }

        f.DestinationPort = packets.front().DestinationPort;
        f.FlowDuration = static_cast<double>(flow.GetLastSeenUs() - flow.GetFirstSeenUs());

        std::vector<double> allPayloadLengths;
        std::vector<double> fwdPayloadLengths;
        std::vector<double> bwdPayloadLengths;

        std::vector<double> flowIat;
        std::vector<double> fwdIat;
        std::vector<double> bwdIat;

        std::vector<double> active;
        std::vector<double> idle;

        std::uint64_t previousTs = 0;
        std::uint64_t previousFwdTs = 0;
        std::uint64_t previousBwdTs = 0;

        std::uint64_t activeStart = packets.front().TimestampUs;
        std::uint64_t activeEnd = packets.front().TimestampUs;

        std::uint64_t subflowCount = 0;
        bool isFirstPacket = true;
        bool firstForwardPacketSeen = false;
        double flowPayloadLengthSum = 0.0;

        for (const FlowPacket &p : packets) {
            const auto payloadLength = static_cast<double>(p.PayloadLength);

            allPayloadLengths.push_back(payloadLength);
            flowPayloadLengthSum += payloadLength;

            if (isFirstPacket) {
                allPayloadLengths.push_back(payloadLength);
                flowPayloadLengthSum += payloadLength;
                isFirstPacket = false;
            }

            if (previousTs != 0 && p.TimestampUs >= previousTs) {
                const auto iat = static_cast<double>(p.TimestampUs - previousTs);
                flowIat.push_back(iat);

                if (iat > SubflowThresholdUs) {
                    ++subflowCount;
                }

                if (iat > static_cast<double>(activeIdleThresholdUs)) {
                    const auto activeDuration = static_cast<double>(activeEnd - activeStart);

                    if (activeDuration > 0.0) {
                        active.push_back(activeDuration);
                    }

                    idle.push_back(iat);
                    activeStart = p.TimestampUs;
                }

                activeEnd = p.TimestampUs;
            }

            previousTs = p.TimestampUs;

            if (f.MinPacketLength == 0 || p.PayloadLength < f.MinPacketLength) {
                f.MinPacketLength = p.PayloadLength;
            }

            f.MaxPacketLength = std::max<std::uint64_t>(f.MaxPacketLength, p.PayloadLength);

            if (p.Flags.Fin) ++f.FINFlagCount;
            if (p.Flags.Syn) ++f.SYNFlagCount;
            if (p.Flags.Rst) ++f.RSTFlagCount;
            if (p.Flags.Psh) ++f.PSHFlagCount;
            if (p.Flags.Ack) ++f.ACKFlagCount;
            if (p.Flags.Urg) ++f.URGFlagCount;
            if (p.Flags.Cwr) ++f.CWEFlagCount;
            if (p.Flags.Ece) ++f.ECEFlagCount;

            if (p.Direction == FlowDirection::Forward) {
                ++f.TotalFwdPackets;
                f.TotalLengthOfFwdPackets += p.PayloadLength;
                f.FwdHeaderLength += p.HeaderLength;

                fwdPayloadLengths.push_back(payloadLength);

                if (f.FwdPacketLengthMin == 0 || p.PayloadLength < f.FwdPacketLengthMin) {
                    f.FwdPacketLengthMin = p.PayloadLength;
                }

                f.FwdPacketLengthMax = std::max<std::uint64_t>(f.FwdPacketLengthMax, p.PayloadLength);

                if (p.Flags.Psh) ++f.FwdPSHFlags;
                if (p.Flags.Urg) ++f.FwdURGFlags;

                if (!firstForwardPacketSeen) {
                    f.InitWinBytesForward = p.WindowSize;
                    firstForwardPacketSeen = true;
                } else if (p.PayloadLength > 0) {
                    ++f.ActDataPktFwd;
                }

                if (f.MinSegSizeForward == 0 || p.HeaderLength < f.MinSegSizeForward) {
                    f.MinSegSizeForward = p.HeaderLength;
                }

                if (previousFwdTs != 0 && p.TimestampUs >= previousFwdTs) {
                    fwdIat.push_back(static_cast<double>(p.TimestampUs - previousFwdTs));
                }

                previousFwdTs = p.TimestampUs;
            } else if (p.Direction == FlowDirection::Backward) {
                ++f.TotalBackwardPackets;
                f.TotalLengthOfBwdPackets += p.PayloadLength;
                f.BwdHeaderLength += p.HeaderLength;

                bwdPayloadLengths.push_back(payloadLength);

                if (f.BwdPacketLengthMin == 0 || p.PayloadLength < f.BwdPacketLengthMin) {
                    f.BwdPacketLengthMin = p.PayloadLength;
                }

                f.BwdPacketLengthMax = std::max<std::uint64_t>(f.BwdPacketLengthMax, p.PayloadLength);

                f.InitWinBytesBackward = p.WindowSize;

                if (previousBwdTs != 0 && p.TimestampUs >= previousBwdTs) {
                    bwdIat.push_back(static_cast<double>(p.TimestampUs - previousBwdTs));
                }

                previousBwdTs = p.TimestampUs;
            }
        }

        f.FwdPacketLengthMean = Mean(fwdPayloadLengths);
        f.FwdPacketLengthStd = Std(fwdPayloadLengths);

        f.BwdPacketLengthMean = Mean(bwdPayloadLengths);
        f.BwdPacketLengthStd = Std(bwdPayloadLengths);

        f.PacketLengthMean = Mean(allPayloadLengths);
        f.PacketLengthVariance = Variance(allPayloadLengths);
        f.PacketLengthStd = std::sqrt(f.PacketLengthVariance);

        f.FlowIATMean = Mean(flowIat);
        f.FlowIATStd = Std(flowIat);
        f.FlowIATMax = Max(flowIat);
        f.FlowIATMin = Min(flowIat);

        f.FwdIATTotal = Sum(fwdIat);
        f.FwdIATMean = Mean(fwdIat);
        f.FwdIATStd = Std(fwdIat);
        f.FwdIATMax = Max(fwdIat);
        f.FwdIATMin = Min(fwdIat);

        f.BwdIATTotal = Sum(bwdIat);
        f.BwdIATMean = Mean(bwdIat);
        f.BwdIATStd = Std(bwdIat);
        f.BwdIATMax = Max(bwdIat);
        f.BwdIATMin = Min(bwdIat);

        const double durationSeconds = f.FlowDuration / MicrosecondsPerSecond;
        const auto totalPackets = static_cast<double>(f.TotalFwdPackets + f.TotalBackwardPackets);
        const auto totalBytes = static_cast<double>(f.TotalLengthOfFwdPackets + f.TotalLengthOfBwdPackets);

        f.FlowBytesPerSecond = Divide(totalBytes, durationSeconds);
        f.FlowPacketsPerSecond = Divide(totalPackets, durationSeconds);
        f.FwdPacketsPerSecond = Divide(static_cast<double>(f.TotalFwdPackets), durationSeconds);
        f.BwdPacketsPerSecond = Divide(static_cast<double>(f.TotalBackwardPackets), durationSeconds);

        f.DownUpRatio = f.TotalFwdPackets == 0
                            ? 0.0
                            : static_cast<double>(f.TotalBackwardPackets) / static_cast<double>(f.TotalFwdPackets);

        f.AveragePacketSize = Divide(flowPayloadLengthSum, totalPackets);
        f.AvgFwdSegmentSize = Mean(fwdPayloadLengths);
        f.AvgBwdSegmentSize = Mean(bwdPayloadLengths);

        f.FwdHeaderLengthDuplicated0 = f.FwdHeaderLength;

        if (subflowCount > 0) {
            f.SubflowFwdPackets = static_cast<double>(f.TotalFwdPackets / subflowCount);
            f.SubflowFwdBytes = static_cast<double>(f.TotalLengthOfFwdPackets / subflowCount);
            f.SubflowBwdPackets = static_cast<double>(f.TotalBackwardPackets / subflowCount);
            f.SubflowBwdBytes = static_cast<double>(f.TotalLengthOfBwdPackets / subflowCount);
        }

        f.ActiveMean = Mean(active);
        f.ActiveStd = Std(active);
        f.ActiveMax = Max(active);
        f.ActiveMin = Min(active);

        f.IdleMean = Mean(idle);
        f.IdleStd = Std(idle);
        f.IdleMax = Max(idle);
        f.IdleMin = Min(idle);

        return f;
    }
} // SCore
