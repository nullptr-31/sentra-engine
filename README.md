# sentra-engine
High performance C++ packet capture and flow analysis engine for the Sentra platform

Sentra Engine supports Linux only because it requires DPDK. CMake rejects
non-Linux target platforms during configuration.

Clang with C++23 support is required. Builds use C++23 with compiler
extensions disabled; falling back to an older standard is not allowed.

Build both the core library and runtime with the build script (requires CMake
4.2 or newer):

```sh
./build.sh Debug      # Default when no configuration is supplied
./build.sh Profile    # Optimized build with debug symbols (RelWithDebInfo)
./build.sh Release
./build.sh all        # Build all three configurations
```

Each configuration uses a separate directory under `build/`: `debug`, `profile`,
or `release`. The core library is at `core/Engine/libsentra-engine.a` and the
runtime executable is at `runtime/sentra-engine` within that directory. The
script works from any working directory and uses `clang++` by default; set `CXX`
to select another Clang installation. Set `CMAKE_BUILD_PARALLEL_LEVEL` to limit
parallel build jobs, for example `CMAKE_BUILD_PARALLEL_LEVEL=4 ./build.sh Debug`.

Alternatively, configure and build directly with Clang:

```sh
cmake -S . -B build-clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build-clang
```

Builds enable stack protection, position-independent code, full RELRO, and a
non-executable stack. Optimized builds also enable libc fortification.
