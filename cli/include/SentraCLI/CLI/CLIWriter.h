//
// Created by Abdelbaki Boukerche on 20/7/2026.
//

#ifndef SENTRA_ENGINE_CLIWRITER_H
#define SENTRA_ENGINE_CLIWRITER_H

#include <Sentra/Capture/CaptureSession.h>

namespace SCLI::Command {
    namespace Show {
        inline void Version() {
            std::cout
                    << "Sentra Intrusion Detection System Software\n"
                    << "Sentra Engine Software, Version " << SENTRA_ENGINE_VERSION << " Experimental\n"
                    << "Compiled " << __DATE__ << " " << __TIME__ << "\n";
        }

        inline void Platform() {
            // TODO(add values): replace hardcoded platform inventory with runtime/platform telemetry.
            std::cout
                    << "Sentra Engine Platform\n"
                    << "Processor board ID SENTRA0000001\n"
                    << "1 virtual CPU\n"
                    << "512M bytes of memory\n"
                    << "\n"
                    << "System image file is \"sentra-engine\"\n"
                    << "Last reload reason: power-on\n"
                    << "\n"
                    << "Packet capture engine: PcapPlusPlus\n"
                    << "Packet decode engine:  Packet++\n"
                    << "\n"
                    << "Sentra Engine uptime is 0 minutes\n"
                    << "System returned to ROM by power-on\n";
        }

        inline void Processes() {
            // TODO(Abdelbaki): replace hardcoded CPU and process counters with runtime telemetry.
            std::cout
                    << "CPU utilization for five seconds: 2%/0%; one minute: 1%; five minutes: 1%\n"
                    << " PID Runtime(ms) Invoked uSecs  Process\n"
                    << "   1          84      128   656  Sentra Init\n"
                    << "   2         312     1042   299  Capture Manager\n"
                    << "   3        1840    58211    31  Packet Processor\n"
                    << "   4         221     3319    66  Flow Manager\n"
                    << "   5          18       72   250  CLI Parser\n"
                    << "   6           0        1     0  Logger\n";
        }

        inline void Clock() {
            // TODO(add values): replace hardcoded clock with system clock formatting.
            std::cout
                    << "*17:42:08.123 UTC Mon Jul 20 2026\n";
        }

        inline void Interfaces() {
            // TODO(add values): replace hardcoded interface inventory with InterfaceManager data.
            std::cout
                    << "Ethernet0 is up, line protocol is up\n"
                    << "  Hardware is Ethernet, address is 4c32.75aa.1022\n"
                    << "  Internet address is 192.168.1.24/24\n"
                    << "  MTU 1500 bytes, BW 1000000 Kbit/sec, DLY 10 usec\n"
                    << "     reliability 255/255, txload 1/255, rxload 4/255\n"
                    << "  Encapsulation ARPA, loopback not set\n"
                    << "  Full-duplex, 1000Mb/s, media type is RJ45\n"
                    << "  Capture is enabled, session 1 active\n"
                    << "  5 minute input rate 42800000 bits/sec, 8421 packets/sec\n"
                    << "  5 minute output rate 1200000 bits/sec, 311 packets/sec\n"
                    << "     2912004 packets input, 184219 packets captured\n"
                    << "     0 input errors, 0 CRC, 0 frame, 0 overrun, 12 ignored\n"
                    << "\n"
                    << "Loopback0 is up, line protocol is up\n"
                    << "  Hardware is Loopback\n"
                    << "  Internet address is 127.0.0.1/8\n"
                    << "  MTU 16384 bytes, BW 8000000 Kbit/sec, DLY 5000 usec\n"
                    << "  Capture is disabled\n"
                    << "  5 minute input rate 0 bits/sec, 0 packets/sec\n"
                    << "  5 minute output rate 0 bits/sec, 0 packets/sec\n";
        }

        inline void Interface(const std::string &name) {
            // TODO(add values): resolve interface by name and print live counters.
            std::cout
                    << name << " is up, line protocol is up\n"
                    << "  Hardware is Ethernet, address is 4c32.75aa.1022\n"
                    << "  Internet address is 192.168.1.24/24\n"
                    << "  MTU 1500 bytes, BW 1000000 Kbit/sec, DLY 10 usec\n"
                    << "     reliability 255/255, txload 1/255, rxload 4/255\n"
                    << "  Encapsulation ARPA, loopback not set\n"
                    << "  Full-duplex, 1000Mb/s, media type is RJ45\n"
                    << "  Capture is enabled, session 1 active\n"
                    << "  Last input 00:00:01, output 00:00:04, output hang never\n"
                    << "  Last clearing of counters never\n"
                    << "  Queueing strategy: fifo\n"
                    << "  Input queue: 0/375/0/0 (size/max/drops/flushes); Total output drops: 0\n"
                    << "  5 minute input rate 42800000 bits/sec, 8421 packets/sec\n"
                    << "  5 minute output rate 1200000 bits/sec, 311 packets/sec\n"
                    << "     2912004 packets input, 184219 packets captured\n"
                    << "     0 input errors, 0 CRC, 0 frame, 0 overrun, 12 ignored\n";
        }

        inline void InterfacesCapture() {
            // TODO(add values): replace hardcoded capture state with CaptureManager sessions per interface.
            std::cout
                    << "Ethernet0 is up, line protocol is up\n"
                    << "  Capture is enabled\n"
                    << "  Capture session 1 is active\n"
                    << "  Capture filter is not set\n"
                    << "  Buffer size is 256 Mbytes, 18 Mbytes used\n"
                    << "  Packets captured 184219, packets dropped 12\n"
                    << "  Last packet received 00:00:01 ago\n"
                    << "\n"
                    << "Loopback0 is up, line protocol is up\n"
                    << "  Capture is disabled\n";
        }

        inline void CaptureSessionsStats(const std::vector<const SCore::CaptureSession *> &sessions) {
            if (sessions.empty()) {
                std::cout << "% No active capture sessions\n";
                return;
            }

            for (const auto *session: sessions) {
                if (session == nullptr) {
                    continue;
                }

                const auto &[PacketsReceived, PacketsQueued, PacketsDropped, PacketsProcessed, ParseErrors] = session->
                        GetStats();

                const std::uint64_t received =
                        PacketsReceived.load(std::memory_order_relaxed);

                const std::uint64_t queued =
                        PacketsQueued.load(std::memory_order_relaxed);

                const std::uint64_t processed =
                        PacketsProcessed.load(std::memory_order_relaxed);

                const std::uint64_t dropped =
                        PacketsDropped.load(std::memory_order_relaxed);

                const std::uint64_t parseErrors =
                        ParseErrors.load(std::memory_order_relaxed);

                std::cout
                        << session->GetInterfaceName() << " is capturing\n"
                        << "  " << received << " packets received, "
                        << queued << " packets queued\n"
                        << "  " << processed << " packets processed, "
                        << dropped << " packets dropped\n"
                        << "  " << parseErrors << " input parse errors\n\n";
            }
        }
    }


    namespace SystemConfig {
    }

    namespace InterfaceConfig {
    }
}

#endif //SENTRA_ENGINE_CLIWRITER_H
