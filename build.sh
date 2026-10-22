#!/bin/sh
set -eu

usage() {
    echo "Usage: $0 [Debug|Profile|Release|all]"
    echo "Defaults to Debug. Profile uses CMake's RelWithDebInfo configuration."
    echo "Set CMAKE_BUILD_PARALLEL_LEVEL to limit parallel build jobs."
}

if [ "$#" -gt 1 ]; then
    usage >&2
    exit 2
fi

case "${1:-Debug}" in
    Debug|debug) configurations=Debug ;;
    Profile|profile) configurations=Profile ;;
    Release|release) configurations=Release ;;
    all|All) configurations="Debug Profile Release" ;;
    -h|--help) usage; exit 0 ;;
    *) usage >&2; exit 2 ;;
esac

if ! command -v cmake >/dev/null 2>&1; then
    echo "Error: CMake 4.2 or newer is required." >&2
    exit 1
fi

project_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)

for configuration in $configurations; do
    case "$configuration" in
        Debug) build_type=Debug; directory=debug ;;
        Profile) build_type=RelWithDebInfo; directory=profile ;;
        Release) build_type=Release; directory=release ;;
    esac
    build_dir="$project_dir/build/$directory"

    echo "Building $configuration (core library and runtime)..."
    cmake -S "$project_dir" -B "$build_dir" \
        "-DCMAKE_CXX_COMPILER=${CXX:-clang++}" \
        "-DCMAKE_BUILD_TYPE=$build_type"
    cmake --build "$build_dir" --config "$build_type" \
        --target sentra-engine sentra-runtime --parallel
done
