add_library(sentra-core STATIC)

add_library(Sentra::Core ALIAS sentra-core)

target_sources(sentra-core
    PRIVATE
        src/Core/Application.cpp
        src/Core/Logger.cpp
        src/Network/InterfaceManager.cpp
        src/Capture/CaptureManager.cpp
        src/Capture/CaptureSession.cpp
        src/Capture/PacketProcessor.cpp
        src/Flow/Flow.cpp
        src/Flow/FlowPacketParser.cpp
        src/Flow/FlowManager.cpp
        include/Sentra/Flow/Flow.h
        include/Sentra/Utils/LoggerTags.h
        include/Sentra/Flow/FlowKey.h
        include/Sentra/Flow/FlowPacket.h
        include/Sentra/Flow/FlowPacketParser.h
        include/Sentra/Flow/FlowManager.h

    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            include/Sentra/Core/Application.h
            include/Sentra/Core/Logger.h
            include/Sentra/Network/InterfaceManager.h
            include/Sentra/Capture/CaptureManager.h
            include/Sentra/Capture/CaptureSession.h
            include/Sentra/Capture/BoundedPacketBuffer.h
            include/Sentra/Capture/PacketProcessor.h
)

# ---------------------------------------------------------
# Dependencies
# ---------------------------------------------------------

set(SPDLOG_BUILD_EXAMPLE OFF)
set(SPDLOG_BUILD_EXAMPLE_HO OFF)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_TESTS_HO OFF)
set(SPDLOG_BUILD_BENCH OFF)
set(SPDLOG_INSTALL OFF)

set(PCAPPP_BUILD_EXAMPLES OFF)
set(PCAPPP_BUILD_TUTORIALS OFF)
set(PCAPPP_BUILD_TESTS OFF)
set(PCAPPP_PACKAGE OFF)
set(PCAPPP_INSTALL OFF)
set(PCAPPP_LOG_LEVEL "Off")
set(PCAPPP_BUILD_COVERAGE OFF)

set(RDKAFKA_BUILD_EXAMPLES OFF)
set(RDKAFKA_BUILD_TESTS OFF)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/PcapPlusPlus)
add_subdirectory(vendor/librdkafka)

target_link_libraries(sentra-core
        PUBLIC
            spdlog::spdlog
            Common++
            Packet++
            Pcap++
            rdkafka++
)