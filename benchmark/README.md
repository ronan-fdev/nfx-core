# Benchmarks

---

## Test Environment

### Hardware Configuration

| Component                | Specification                                                 |
| ------------------------ | ------------------------------------------------------------- |
| **CPU**                  | 12th Gen Intel Core i7-12800H (20 logical, 14 physical cores) |
| **Base Clock**           | 2.80 GHz                                                      |
| **Turbo Clock**          | 4.80 GHz                                                      |
| **L1 Data Cache**        | 48 KiB (×6 P-cores) + 32 KiB (×8 E-cores)                     |
| **L1 Instruction Cache** | 32 KiB (×6 P-cores) + 64 KiB (×2 E-core clusters)             |
| **L2 Unified Cache**     | 1.25 MiB (×6 P-cores) + 2 MiB (×2 E-core clusters)            |
| **L3 Unified Cache**     | 24 MiB (×1 shared)                                            |
| **RAM**                  | DDR4-3200 (32GB)                                              |
| **GPU**                  | NVIDIA RTX A2000 4GB GDDR6                                    |

### Software Configuration

| Platform    | Benchmark Framework     | C++ Compiler              | nfx-core Version |
| ----------- | ----------------------- | ------------------------- | ---------------- |
| **Linux**   | Google Benchmark v1.9.4 | GCC 14.2.0-x64            | v1.0.0           |
| **Linux**   | Google Benchmark v1.9.4 | Clang 19.1.7-x64          | v1.0.0           |
| **Windows** | Google Benchmark v1.9.4 | MinGW GCC 14.2.0-x64      | v1.0.0           |
| **Windows** | Google Benchmark v1.9.4 | Clang-GNU-CLI 19.1.5-x64  | v1.0.0           |
| **Windows** | Google Benchmark v1.9.4 | Clang-MSVC-CLI 19.1.5-x64 | v1.0.0           |
| **Windows** | Google Benchmark v1.9.4 | MSVC 19.44.35217.0-x64    | v1.0.0           |

---

# Performance Results

## String Hashing Operations

### Short Strings (10 chars)

| Operation                        | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| -------------------------------- | ----------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| `hashStringView()` (auto CRC32)  | **216 ns**  | 267 ns      | 229 ns            | 369 ns                | 495 ns                 | 328 ns       |
| Manual FNV-1a                    | **208 ns**  | 350 ns      | 225 ns            | 356 ns                | 484 ns                 | 229 ns       |
| Manual CRC32 (SSE4.2)            | **207 ns**  | 238 ns      | 218 ns            | 308 ns                | 447 ns                 | 249 ns       |
| `std::hash<std::string_view>`    | 362 ns      | 405 ns      | **303 ns**        | 330 ns                | 445 ns                 | 357 ns       |
| Single string `hashStringView()` | 9.58 ns     | **8.69 ns** | 11.6 ns           | 13.3 ns               | 16.9 ns                | 10.8 ns      |
| Single string `std::hash`        | **4.70 ns** | 4.84 ns     | 5.21 ns           | 20.1 ns               | 25.9 ns                | 15.3 ns      |

### Medium Strings (50 chars)

| Operation                       | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------- | --------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| `hashStringView()` (auto CRC32) | 502 ns    | **447 ns**  | 536 ns            | 558 ns                | 820 ns                 | 600 ns       |
| Manual FNV-1a                   | 624 ns    | **601 ns**  | 684 ns            | 808 ns                | 1014 ns                | 656 ns       |
| Manual CRC32 (SSE4.2)           | 496 ns    | **438 ns**  | 1147 ns           | 609 ns                | 837 ns                 | 536 ns       |
| `std::hash<std::string_view>`   | 419 ns    | 463 ns      | **413 ns**        | 863 ns                | 1151 ns                | 656 ns       |

### Long Strings (1000 chars)

| Operation                       | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------- | ----------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| `hashStringView()` (auto CRC32) | 4923 ns     | **3955 ns** | 5885 ns           | 5534 ns               | 8231 ns                | 5156 ns      |
| Manual FNV-1a                   | 7385 ns     | **7198 ns** | 8777 ns           | 10824 ns              | 13602 ns               | 7533 ns      |
| Manual CRC32 (SSE4.2)           | 4840 ns     | **3921 ns** | 5766 ns           | 5952 ns               | 8045 ns                | 5052 ns      |
| `std::hash<std::string_view>`   | **1115 ns** | 1249 ns     | 1266 ns           | 11254 ns              | 15381 ns               | 7615 ns      |

---

## Integer Hashing Operations

| Operation                 | Linux GCC  | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------- | ---------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| `hashInteger<uint32_t>()` | **360 ns** | 579 ns      | 405 ns            | 802 ns                | 1087 ns                | 651 ns       |
| `hashInteger<uint64_t>()` | **732 ns** | 1135 ns     | 825 ns            | 1697 ns               | 2148 ns                | 830 ns       |
| `hashInteger<int32_t>()`  | **357 ns** | 561 ns      | 414 ns            | 825 ns                | 1032 ns                | 637 ns       |
| `std::hash<uint32_t>`     | **107 ns** | 107 ns      | 119 ns            | 2407 ns               | 3026 ns                | 984 ns       |
| `std::hash<uint64_t>`     | 295 ns     | **188 ns**  | 327 ns            | 3159 ns               | 3836 ns                | 1874 ns      |

---

## Hash Combining & Seed Mixing

| Operation        | Linux GCC    | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------- | ------------ | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| `seedMix()`      | **0.106 ns** | 0.221 ns    | 0.124 ns          | 0.297 ns              | 0.436 ns               | 0.698 ns     |
| Low-level FNV-1a | **0.106 ns** | 0.215 ns    | 0.124 ns          | 0.273 ns              | 0.411 ns               | 0.709 ns     |
| Low-level CRC32  | **0.107 ns** | 0.217 ns    | 0.123 ns          | 0.283 ns              | 0.417 ns               | 0.715 ns     |
| Low-level Larson | **0.106 ns** | 0.215 ns    | 0.124 ns          | 0.287 ns              | 0.424 ns               | 0.703 ns     |

---

## Cache-Sensitive Patterns

| Operation               | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------- | ----------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| Sequential 100 strings  | 3561 ns     | **2374 ns** | 3476 ns           | 3555 ns               | 4383 ns                | 3683 ns      |
| Random 100 strings      | 9320 ns     | **4978 ns** | 10661 ns          | 7208 ns               | 8371 ns                | 9556 ns      |
| Cache test (20 repeats) | **2214 ns** | 2322 ns     | 2530 ns           | 3982 ns               | 4778 ns                | 3345 ns      |

---

_Benchmarks executed on November 1, 2025_
