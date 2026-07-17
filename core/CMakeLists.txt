add_library(sentra-core STATIC)

add_library(Sentra::Core ALIAS sentra-core)

target_sources(sentra-core
    PRIVATE
        src/Core/Application.cpp
        src/Core/Logger.cpp

    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            include/Sentra/Core/Application.h
            include/Sentra/Core/Logger.h
)

target_include_directories(sentra-core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ---------------------------------------------------------
# Dependencies
# ---------------------------------------------------------

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE_HO OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS_HO OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/spdlog)

target_link_libraries(sentra-core
        PUBLIC
            spdlog::spdlog
)