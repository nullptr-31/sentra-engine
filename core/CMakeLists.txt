add_library(sentra-core STATIC)

add_library(Sentra::Core ALIAS sentra-core)

target_sources(sentra-core
    PRIVATE
        src/Core/Application.cpp

    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS include
        FILES
            include/Sentra/Core/Application.h
)

target_include_directories(sentra-core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)