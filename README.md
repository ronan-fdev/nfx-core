# nfx-core

<!-- Project Info -->

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://github.com/ronan-fdev/nfx-core/blob/main/LICENSE.txt) [![GitHub release (latest by date)](https://img.shields.io/github/v/release/ronan-fdev/nfx-core?style=flat-square)](https://github.com/ronan-fdev/nfx-core/releases) [![GitHub tag (latest by date)](https://img.shields.io/github/tag/ronan-fdev/nfx-core?style=flat-square)](https://github.com/ronan-fdev/nfx-core/tags)<br/>

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?style=flat-square) ![CMake](https://img.shields.io/badge/CMake-3.20%2B-green.svg?style=flat-square) ![Cross Platform](https://img.shields.io/badge/Platform-Linux_Windows-lightgrey?style=flat-square)

<!-- CI/CD Status -->

[![Linux GCC](https://img.shields.io/github/actions/workflow/status/ronan-fdev/nfx-core/build-linux-gcc.yml?branch=main&label=Linux%20GCC&style=flat-square)](https://github.com/ronan-fdev/nfx-core/actions/workflows/build-linux-gcc.yml) [![Linux Clang](https://img.shields.io/github/actions/workflow/status/ronan-fdev/nfx-core/build-linux-clang.yml?branch=main&label=Linux%20Clang&style=flat-square)](https://github.com/ronan-fdev/nfx-core/actions/workflows/build-linux-clang.yml) [![Windows MinGW](https://img.shields.io/github/actions/workflow/status/ronan-fdev/nfx-core/build-windows-mingw.yml?branch=main&label=Windows%20MinGW&style=flat-square)](https://github.com/ronan-fdev/nfx-core/actions/workflows/build-windows-mingw.yml) [![Windows MSVC](https://img.shields.io/github/actions/workflow/status/ronan-fdev/nfx-core/build-windows-msvc.yml?branch=main&label=Windows%20MSVC&style=flat-square)](https://github.com/ronan-fdev/nfx-core/actions/workflows/build-windows-msvc.yml) [![CodeQL](https://img.shields.io/github/actions/workflow/status/ronan-fdev/nfx-core/codeql.yml?branch=main&label=CodeQL&style=flat-square)](https://github.com/ronan-fdev/nfx-core/actions/workflows/codeql.yml)

> Foundation of the nfx ecosystem - A modern C++20 header-only library providing core utilities with high-performance hashing algorithms and CPU feature detection

## Overview

nfx-core is the foundational library of the nfx ecosystem, providing essential core utilities optimized for performance across multiple platforms and compilers. It delivers hardware-accelerated hashing algorithms with automatic CPU feature detection, enabling optimal algorithm selection at runtime. Built with modern C++20, the library offers zero-cost abstractions, constexpr support, and cross-platform compatibility.

## Features

### ⚡ High-Performance Hashing Algorithms

- **Hardware Acceleration**: SSE4.2 CRC32 intrinsics for 3-5x faster hashing
- **Software Fallback**: FNV-1a algorithm for systems without SSE4.2
- **Multiple Algorithms**: CRC32, FNV-1a, Larson, integer hashing (32/64-bit)
- **Hash Combining**: Boost-style + MurmurHash3 finalizer for composite keys
- **Seed Mixing**: Utilities for hash table probing and collision resolution
- **Constexpr Support**: Compile-time hash computation where possible

### 🧠 CPU Feature Detection

- **SSE4.2 Detection**: Enables hardware-accelerated CRC32 hashing
- **AVX Detection**: 256-bit floating-point SIMD operations support
- **AVX2 Detection**: 256-bit integer SIMD operations support
- **Static Initialization**: Zero runtime overhead with compile-time detection
- **Cross-Platform**: Works with GCC, Clang and MSVC intrinsics

### 📊 Real-World Applications

- **Hash Tables**: Foundation for custom hash table implementations
- **Data Structures**: High-performance key hashing for maps and sets
- **Checksums**: Fast data integrity verification
- **Fingerprinting**: Quick data fingerprint generation

### 🎯 Algorithm Selection

- **Adaptive**: Automatically selects best algorithm based on CPU capabilities
- **Type-Safe**: Template-based integer hashing with compile-time type checking
- **Optimized**: Knuth's multiplicative hashing (32-bit), Wang's avalanche (64-bit)
- **Avalanche**: Excellent bit distribution for uniform hash values

### ⚡ Performance Optimized

- Header-only library with zero runtime dependencies
- Hardware-accelerated hashing with SSE4.2 intrinsics
- Efficient hash combining with excellent avalanche properties
- Zero-cost abstractions with constexpr support
- Compiler-optimized inline implementations
- Static CPU feature detection (zero runtime overhead)

### 🌍 Cross-Platform Support

- Linux, Windows
- GCC 14+, Clang 19+, MSVC 2022+
- Thread-safe operations
- Consistent behavior across platforms
- Comprehensive CI/CD testing on multiple compilers

## Quick Start

### Requirements

- C++20 compatible compiler:
  - **GCC 14+** (14.2.0 tested)
  - **Clang 18+** (19.1.7 tested)
  - **MSVC 2022+** (19.44+ tested)
- CMake 3.20 or higher

### CMake Integration

```cmake
# Development options
option(NFX_CORE_BUILD_TESTS          "Build tests"                        OFF )
option(NFX_CORE_BUILD_SAMPLES        "Build samples"                      OFF )
option(NFX_CORE_BUILD_BENCHMARKS     "Build benchmarks"                   OFF )
option(NFX_CORE_BUILD_DOCUMENTATION  "Build Doxygen documentation"        OFF )

# Installation and packaging
option(NFX_CORE_INSTALL_PROJECT      "Install project"                    OFF )
option(NFX_CORE_PACKAGE_SOURCE       "Enable source package generation"   OFF )
option(NFX_CORE_PACKAGE_ARCHIVE      "Enable TGZ/ZIP package generation"  OFF )
option(NFX_CORE_PACKAGE_DEB          "Enable DEB package generation"      OFF )
option(NFX_CORE_PACKAGE_RPM          "Enable RPM package generation"      OFF )
option(NFX_CORE_PACKAGE_WIX          "Enable WiX MSI installer"           OFF )
```

### Using in Your Project

#### Option 1: Using FetchContent (Recommended)

```cmake
include(FetchContent)
FetchContent_Declare(
  nfx-core
  GIT_REPOSITORY https://github.com/ronan-fdev/nfx-core.git
  GIT_TAG        main  # or use specific version tag like "1.0.0"
)
FetchContent_MakeAvailable(nfx-core)

# Link with header-only interface library
target_link_libraries(your_target PRIVATE nfx-core::nfx-core)
```

#### Option 2: As a Git Submodule

```bash
# Add as submodule
git submodule add https://github.com/ronan-fdev/nfx-core.git third-party/nfx-core
```

```cmake
# In your CMakeLists.txt
add_subdirectory(third-party/nfx-core)
target_link_libraries(your_target PRIVATE nfx-core::nfx-core)
```

#### Option 3: Using find_package (After Installation)

```cmake
find_package(nfx-core REQUIRED)
target_link_libraries(your_target PRIVATE nfx-core::nfx-core)
```

### Building

```bash
# Clone the repository
git clone https://github.com/ronan-fdev/nfx-core.git
cd nfx-core

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the library
cmake --build . --config Release --parallel

# Run tests (optional)
ctest -C Release --output-on-failure

# Run benchmarks (optional)
./build/bin/benchmarks/BM_Hashing
```

### Documentation

nfx-core includes comprehensive API documentation generated with Doxygen.

#### 📚 Online Documentation

The complete API documentation is available online at:
**https://ronan-fdev.github.io/nfx-core**

#### Building Documentation Locally

```bash
# Configure with documentation enabled
cmake .. -DCMAKE_BUILD_TYPE=Release -DNFX_CORE_BUILD_DOCUMENTATION=ON

# Build the documentation
cmake --build . --target documentation
```

#### Requirements

- **Doxygen** - Documentation generation tool
- **Graphviz Dot** (optional) - For generating class diagrams

#### Accessing Local Documentation

After building, open `./build/doc/html/index.html` in your web browser.

## Usage Example

```cpp
#include <nfx/core/Hashing.h>
#include <nfx/core/CPU.h>

using namespace nfx::core;

// Hash a string with FNV-1a
uint32_t hash = hashing::hashStringView
    <hashing::constants::DEFAULT_FNV_OFFSET_BASIS, hashing::constants::DEFAULT_FNV_PRIME>("example_key");

// Check CPU capabilities
if (cpu::hasSSE42Support())
{
    // Use hardware-accelerated CRC32
}
```

## Installation & Packaging

nfx-core provides comprehensive packaging options for distribution.

### Package Generation

```bash
# Configure with packaging options
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DNFX_CORE_PACKAGE_ARCHIVE=ON \
         -DNFX_CORE_PACKAGE_DEB=ON \
         -DNFX_CORE_PACKAGE_RPM=ON

# Generate binary packages
cmake --build . --target package
# or
cd build && cpack

# Generate source packages
cd build && cpack --config CPackSourceConfig.cmake
```

### Supported Package Formats

| Format      | Platform       | Description                        | Requirements |
| ----------- | -------------- | ---------------------------------- | ------------ |
| **TGZ/ZIP** | Cross-platform | Compressed archive packages        | None         |
| **DEB**     | Debian/Ubuntu  | Native Debian packages             | `dpkg-dev`   |
| **RPM**     | RedHat/SUSE    | Native RPM packages                | `rpm-build`  |
| **WiX**     | Windows        | Professional MSI installer         | `WiX 3.11+`  |
| **Source**  | Cross-platform | Source code distribution (TGZ+ZIP) | None         |

### Installation

```bash
# Linux (DEB-based systems)
sudo dpkg -i nfx-core_*_amd64.deb

# Linux (RPM-based systems)
sudo rpm -ivh nfx-core-*-Linux.rpm

# Windows
# Run the .exe installer with administrator privileges
nfx-core-*-win64.exe

# Manual installation (extract archive)
tar -xzf nfx-core-*-Linux.tar.gz -C /usr/local/
```

## Project Structure

```
nfx-core/
├── benchmark/             # Benchmarks with Google Benchmark
├── cmake/                 # CMake modules and configuration
├── include/nfx/           # Public headers: core hashing and CPU detection
├── samples/               # Example usage and demonstrations
└── test/                  # Unit tests with GoogleTest
```

## Performance

nfx-core is optimized for high performance with:

- **Hardware-accelerated hashing** - SSE4.2 CRC32 intrinsics provide 3-5x speedup
- **Automatic algorithm selection** - Runtime CPU detection chooses optimal implementation
- **Constexpr support** - Compile-time hash computation where possible
- **Zero-cost abstractions** - No runtime overhead for type safety
- **Optimized avalanche** - Excellent bit distribution for uniform hash values
- **Static detection** - Compile-time initialization eliminates repeated CPU checks
- **Cache-friendly** - Optimized memory access patterns for large datasets

For detailed performance metrics and benchmarks, see the [benchmark documentation](benchmark/README.md).

## Changelog

See the [changelog](CHANGELOG.md) for a detailed history of changes, new features, and bug fixes.

## License

This project is licensed under the MIT License.

## Dependencies

- **[GoogleTest](https://github.com/google/googletest)**: Testing framework (BSD 3-Clause License) - Development only
- **[Google Benchmark](https://github.com/google/benchmark)**: Performance benchmarking framework (Apache 2.0 License) - Development only

All dependencies are automatically fetched via CMake FetchContent when building tests or benchmarks.

---

_Updated on November 1, 2025_
