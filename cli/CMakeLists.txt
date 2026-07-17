add_library(sentra-cli STATIC)

add_library(Sentra::CLI ALIAS sentra-cli)

target_sources(sentra-cli
        PRIVATE
        src/CLI/CLI.cpp

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