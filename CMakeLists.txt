cmake_minimum_required(VERSION 4.2)

project(sentra-engine
        VERSION 0.1.0
        DESCRIPTION "High-performance packet-processing core for Sentra"
        LANGUAGES CXX
)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR
            "Sentra Engine requires Linux because it depends on DPDK. "
            "Unsupported target platform: ${CMAKE_SYSTEM_NAME}"
    )
endif ()

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "Sentra Engine requires Clang.")
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(core)
add_subdirectory(runtime)

include(CheckPIESupported)
check_pie_supported(LANGUAGES CXX)

if (NOT CMAKE_CXX_LINK_PIE_SUPPORTED)
    message(FATAL_ERROR "Sentra Engine requires PIE support.")
endif ()

foreach (target IN ITEMS sentra-engine sentra-runtime)
    set_target_properties(${target} PROPERTIES
            POSITION_INDEPENDENT_CODE ON
    )

    target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wformat=2
            -Werror=format-security
            -fstack-protector-strong
    )

    target_compile_definitions(${target} PRIVATE
            "$<$<CONFIG:Release,RelWithDebInfo,MinSizeRel>:_FORTIFY_SOURCE=2>"
    )
endforeach ()

target_link_options(sentra-runtime PRIVATE
        "LINKER:-z,relro"
        "LINKER:-z,now"
        "LINKER:-z,noexecstack"
)
