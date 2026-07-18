add_library(sentra-core STATIC)

add_library(Sentra::Core ALIAS sentra-core)

target_sources(sentra-core
    PRIVATE
        src/Core/Application.cpp
        src/Core/Logger.cpp
        src/Network/InterfaceManager.cpp
        include/Sentra/Utils/LoggerTags.h

    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            include/Sentra/Core/Application.h
            include/Sentra/Core/Logger.h
            include/Sentra/Network/InterfaceManager.h
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

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/PcapPlusPlus)

target_link_libraries(sentra-core
        PUBLIC
            spdlog::spdlog
            Common++
            Packet++
            Pcap++
)