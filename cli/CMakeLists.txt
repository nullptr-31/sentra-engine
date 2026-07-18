add_library(sentra-cli STATIC)

add_library(Sentra::CLI ALIAS sentra-cli)

target_sources(sentra-cli
        PRIVATE
            src/CLI/CLI.cpp
            src/CLI/CLIContext.cpp
            include/SentraCLI/CLI/CLIContext.h
            include/SentraCLI/Command/CommandToken.h
            include/SentraCLI/Utils/StringUtils.h

        PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS include
        FILES
            include/SentraCLI/CLI/CLI.h
)

target_include_directories(sentra-cli
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(sentra-cli
        PRIVATE
            sentra-core
)

# ---------------------------------------------------------
# Dependencies
# ---------------------------------------------------------

set(REPLXX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(REPLXX_BUILD_PACKAGE OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/replxx)

target_link_libraries(sentra-cli
    PRIVATE
        replxx
)