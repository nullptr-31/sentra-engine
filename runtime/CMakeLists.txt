if (NOT TARGET Sentra::Engine)
    message(FATAL_ERROR
            "Sentra::Engine must be added before sentra-runtime"
    )
endif ()

add_executable(sentra-runtime)
add_executable(Sentra::Runtime ALIAS sentra-runtime)

target_compile_features(sentra-runtime
        PRIVATE
        cxx_std_23
)

target_sources(sentra-runtime
        PRIVATE
        src/main.cpp
)

target_link_libraries(sentra-runtime
        PRIVATE
        Sentra::Engine
)

set_target_properties(sentra-runtime
        PROPERTIES
        OUTPUT_NAME sentra-engine
)