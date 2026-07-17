project(sentra-runtime)

add_executable(sentra-runtime)

target_sources(sentra-runtime
    PRIVATE
        src/main.cpp
)

set_target_properties(sentra-runtime
    PROPERTIES
        OUTPUT_NAME sentra-engine
)

target_link_libraries(sentra-runtime
        PRIVATE
        sentra-core
)

target_link_libraries(sentra-runtime
        PRIVATE
            sentra-cli
)