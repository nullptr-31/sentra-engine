# sentra-engine
High performance C++ packet capture and flow analysis engine for the Sentra platform

Sentra Engine supports Linux only because it requires DPDK. CMake rejects
non-Linux target platforms during configuration.

Clang with C++23 support is required. Builds use C++23 with compiler
extensions disabled; falling back to an older standard is not allowed.

Configure and build with Clang:

```sh
cmake -S . -B build-clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build-clang
```

Builds enable stack protection, position-independent code, full RELRO, and a
non-executable stack. Optimized builds also enable libc fortification.
