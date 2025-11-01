/*
 * MIT License
 *
 * Copyright (c) 2025 nfx
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file CPU.h
 * @brief CPU feature detection and capabilities
 * @details Runtime detection of processor features including SSE4.2, AVX2, and other
 *          instruction set extensions for optimized algorithm selection
 */

#pragma once

namespace nfx::core::cpu
{
	//=====================================================================
	// CPU feature detection
	//=====================================================================

	//----------------------------
	// SSE4.2 Detection
	//----------------------------

	/**
	 * @brief Gets the cached SSE4.2 support status.
	 * @details Checks CPU capabilities for SSE4.2 CRC32 instructions, which provide
	 *          3-5x faster hashing compared to software fallback. Result is cached
	 *          via static initialization for zero runtime overhead.
	 * @return `true` if SSE4.2 is supported, `false` otherwise.
	 * @note This function is marked [[nodiscard]] - the return value should not be ignored
	 * @note Requires CPUID leaf 1, ECX bit 20
	 */
	[[nodiscard]] inline bool hasSSE42Support() noexcept;

	//----------------------------
	// AVX Detection
	//----------------------------

	/**
	 * @brief Gets the cached AVX support status.
	 * @details Checks CPU capabilities for AVX (Advanced Vector Extensions) instructions,
	 *          which provide 256-bit SIMD operations for floating-point processing. AVX enables:
	 *          - 256-bit floating-point operations (vs 128-bit SSE)
	 *          - Vectorized mathematical computations
	 *          - SIMD-accelerated floating-point algorithms
	 *          Result is cached via static initialization for zero runtime overhead.
	 * @return `true` if AVX is supported, `false` otherwise.
	 * @note This function is marked [[nodiscard]] - the return value should not be ignored
	 * @note Requires CPUID leaf 1, ECX bit 28
	 */
	[[nodiscard]] inline bool hasAVXSupport() noexcept;

	//----------------------------
	// AVX2 Detection
	//----------------------------

	/**
	 * @brief Gets the cached AVX2 support status.
	 * @details Checks CPU capabilities for AVX2 (Advanced Vector Extensions 2) instructions,
	 *          which provide 256-bit SIMD operations for vectorized processing. AVX2 enables:
	 *          - 256-bit integer operations (vs 128-bit SSE)
	 *          - Vectorized string processing and comparison
	 *          - Parallel hash computation for multiple keys
	 *          - SIMD-accelerated mathematical operations
	 *          Result is cached via static initialization for zero runtime overhead.
	 * @return `true` if AVX2 is supported, `false` otherwise.
	 * @note This function is marked [[nodiscard]] - the return value should not be ignored
	 * @note Requires CPUID leaf 7, subfunction 0, EBX bit 5
	 */
	[[nodiscard]] inline bool hasAVX2Support() noexcept;
} // namespace nfx::core::cpu

#include "nfx/detail/core/CPU.inl"
