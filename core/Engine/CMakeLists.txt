add_library(sentra-engine STATIC)
add_library(Sentra::Engine ALIAS sentra-engine)

target_compile_features(sentra-engine
        PUBLIC
        cxx_std_23
)

target_sources(sentra-engine
        PRIVATE
        src/Engine.cpp

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/Sentra/Engine/Engine.h
)