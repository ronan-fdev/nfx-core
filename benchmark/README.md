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

| Operation                        | Linux GCC  | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| -------------------------------- | ---------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| `hashStringView()` (auto CRC32)  | 250 ns     | **240 ns**  | 272 ns            | 261 ns                | 435 ns                 | 523 ns       |
| Manual FNV-1a                    | **202 ns** | 333 ns      | 270 ns            | 414 ns                | 432 ns                 | 389 ns       |
| Manual CRC32 (SSE4.2)            | **203 ns** | 248 ns      | 285 ns            | 332 ns                | 342 ns                 | 345 ns       |
| `std::hash<std::string_view>`    | 367 ns     | 423 ns      | 414 ns            | **285 ns**            | 353 ns                 | 392 ns       |
| Single string `hashStringView()` | 17.6 ns    | **13.8 ns** | 28.7 ns           | 19.7 ns               | 21.4 ns                | 17.6 ns      |
| Single string `std::hash`        | 4.87 ns    | **4.56 ns** | 8.49 ns           | 19.0 ns               | 20.2 ns                | 23.9 ns      |

### Medium Strings (50 chars)

| Operation                       | Linux GCC  | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------- | ---------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| `hashStringView()` (auto CRC32) | 687 ns     | **631 ns**  | 885 ns            | 767 ns                | 1022 ns                | 995 ns       |
| Manual FNV-1a                   | 646 ns     | **589 ns**  | 884 ns            | 718 ns                | 948 ns                 | 1029 ns      |
| Manual CRC32 (SSE4.2)           | 649 ns     | **598 ns**  | 961 ns            | 739 ns                | 912 ns                 | 896 ns       |
| `std::hash<std::string_view>`   | **403 ns** | 436 ns      | 598 ns            | 767 ns                | 932 ns                 | 991 ns       |

### Long Strings (1000 chars)

| Operation                       | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------- | ----------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| `hashStringView()` (auto CRC32) | 8389 ns     | **7222 ns** | 10820 ns          | 9812 ns               | 12691 ns               | 8970 ns      |
| Manual FNV-1a                   | 8487 ns     | **7164 ns** | 11627 ns          | 9868 ns               | 12510 ns               | 13292 ns     |
| Manual CRC32 (SSE4.2)           | 8553 ns     | **7243 ns** | 12374 ns          | 9923 ns               | 12025 ns               | 8970 ns      |
| `std::hash<std::string_view>`   | **1161 ns** | 1236 ns     | 1850 ns           | 10272 ns              | 12296 ns               | 12796 ns     |

---

## Integer Hashing Operations

| Operation                 | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------- | ----------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| `hashInteger<uint32_t>()` | 1041 ns     | **645 ns**  | 1642 ns           | 875 ns                | 1014 ns                | 1030 ns      |
| `hashInteger<uint64_t>()` | **784 ns**  | 1009 ns     | 1272 ns           | 1383 ns               | 1602 ns                | 1194 ns      |
| `hashInteger<int32_t>()`  | 1057 ns     | **632 ns**  | 1685 ns           | 899 ns                | 1011 ns                | 1028 ns      |
| `std::hash<uint32_t>`     | **95.9 ns** | 117 ns      | 156 ns            | 1585 ns               | 1819 ns                | 1472 ns      |
| `std::hash<uint64_t>`     | 323 ns      | **90.8 ns** | 537 ns            | 2866 ns               | 3278 ns                | 2929 ns      |

---

## Hash Combining & Seed Mixing

| Operation        | Linux GCC    | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------- | ------------ | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| `seedMix()`      | **0.119 ns** | 0.213 ns    | 0.150 ns          | 0.280 ns              | 0.378 ns               | 1.14 ns      |
| Low-level FNV-1a | **0.108 ns** | 0.211 ns    | 0.142 ns          | 0.273 ns              | 0.391 ns               | 1.12 ns      |
| Low-level CRC32  | **0.107 ns** | 0.213 ns    | 0.145 ns          | 0.273 ns              | 0.388 ns               | 1.15 ns      |
| Low-level Larson | **0.107 ns** | 0.214 ns    | 0.146 ns          | 0.276 ns              | 0.375 ns               | 1.13 ns      |

---

## Cache-Sensitive Patterns

| Operation               | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------- | --------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| Sequential 100 strings  | 3706 ns   | **3067 ns** | 5883 ns           | 4122 ns               | 4379 ns                | 3788 ns      |
| Random 100 strings      | 10943 ns  | **7515 ns** | 17785 ns          | 11448 ns              | 11932 ns               | 15991 ns     |
| Cache test (20 repeats) | 2367 ns   | **2155 ns** | 3998 ns           | 2931 ns               | 2935 ns                | 4772 ns      |

---

_Benchmarks executed on November 1, 2025_
