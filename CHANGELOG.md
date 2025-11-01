# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- NIL

### Changed

- **Hashing**

  - **BREAKING**: `hashStringView()` now always uses CRC32-C (no longer switches to FNV-1a)
  - Added software CRC32-C fallback implementation (reflected polynomial 0x82F63B78)
  - Removed runtime CPU detection - all algorithm selection now compile-time
  - Hash consistency guaranteed: same build produces same hash values regardless of runtime CPU
  - Software and hardware CRC32-C implementations produce identical results
  - Template parameters now have defaults for better ergonomics:
    - `fnv1a<>()` - FNV prime defaults to 0x01000193
    - `seedMix<>()` - Mix constant defaults to 0x2545F4914F6CDD1D
    - `hashStringView<>()` - Initial hash defaults to 0x811C9DC5
  - Updated documentation to accurately reflect CRC32-C implementation strategy

- **Benchmarks**

  - Added SSE4.2 compiler flags (-msse4.2 for GCC/Clang, /arch:AVX for MSVC)
  - Updated benchmark/README.md with complete results

- **Tests**

  - Added SSE4.2 compiler flags to test/CMakeLists.txt

- **README.md**

  - Added "Performance Optimization" section documenting SSE4.2 compiler flags
  - Instructions for -msse4.2 (GCC/Clang) and /arch:AVX (MSVC)
  - CMake example showing how to enable hardware acceleration

  - **GitHub Actions Workflows**

  - `build-linux-gcc.yml` and `build-linux-clang.yml` now use `ubuntu-latest` instead of `ubuntu-24.04`
  - `documentation.yml` trigger changed from `release: [published]` to `push: main` with path filters to prevent GitHub Pages protection errors
  - `documentation.yml` now automatically rebuilds when documentation-related files are modified (include/**, doc/**, README.md, CHANGELOG.md)

### Deprecated

- NIL

### Removed

- NIL

### Fixed

- GitHub Pages deployment errors when publishing releases from tags

### Security

- NIL

## [1.0.0] - 2025-11-01 - Initial Release

### Added

- **Core Hashing Algorithms**: High-performance hash functions with hardware acceleration

  - CRC32 hash using SSE4.2 intrinsics
  - FNV-1a 32-bit hash algorithm (software fallback)
  - Larson multiplicative hash
  - Integer hashing (32-bit Knuth, 64-bit Wang avalanche)
  - Hash combining (Boost + MurmurHash3 finalizer)
  - Seed mixing utilities for hash table probing
  - Adaptive algorithm selection based on CPU capabilities
  - All functions use `constexpr` where possible for compile-time optimization

- **CPU Feature Detection**: Runtime hardware capability detection

  - SSE4.2 support detection (enables CRC32 acceleration)
  - AVX support detection (256-bit floating-point operations)
  - AVX2 support detection (256-bit integer SIMD operations)
  - Static initialization for zero runtime overhead
  - Cross-platform CPUID intrinsics (GCC/Clang & MSVC)

- **Build System**

  - CMake 3.20+ configuration with modular structure
  - Header-only interface library target
  - Cross-platform support (Linux GCC/Clang, Windows MinGW/Clang/MSVC)
  - Google Test integration for unit testing
  - Google Benchmark integration for performance benchmarking
  - Doxygen documentation generation with GitHub Pages deployment
  - CPack packaging for distribution (DEB, RPM, TGZ, ZIP, WiX MSI)
  - Installation support with CMake package config files

- **Documentation**

  - README with feature overview and usage examples
  - Detailed API documentation with Doxygen comments and RFC references
  - Sample applications
  - Build and installation instructions

- **Testing & Benchmarking**
  - Unit test suite
  - Performance benchmarks for all operations
  - Cross-compiler performance validation
