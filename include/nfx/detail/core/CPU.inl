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
 * @file CPU.inl
 * @brief CPU feature detection implementation
 * @details Runtime detection of processor features including SSE4.2, AVX, and AVX2
 *          instruction set extensions for optimized algorithm selection
 */

#if defined( __GNUC__ )
#	include <cpuid.h>
#endif
#if defined( _MSC_VER ) || defined( __SSE4_2__ )
#	include <nmmintrin.h>
#endif

#include <array>

namespace nfx::core::cpu
{
	//=====================================================================
	// CPU feature detection
	//=====================================================================

	//----------------------------
	// SSE4.2 Detection
	//----------------------------

	inline bool hasSSE42Support() noexcept
	{
		static const bool s_hasSSE42 = []() {
			bool hasSupport = false;
#if defined( _MSC_VER )
			std::array<int, 4> cpuInfo{};
			__cpuid( cpuInfo.data(), 1 );
			hasSupport = ( cpuInfo[2] & ( 1 << 20 ) ) != 0;
#elif defined( __GNUC__ )
			unsigned int eax, ebx, ecx, edx;
			if ( __get_cpuid( 1, &eax, &ebx, &ecx, &edx ) )
			{
				hasSupport = ( ecx & ( 1 << 20 ) ) != 0;
			}
#endif
			return hasSupport;
		}();

		return s_hasSSE42;
	}

	//----------------------------
	// AVX Detection
	//----------------------------

	inline bool hasAVXSupport() noexcept
	{
		static const bool s_hasAVX = []() {
			bool hasSupport = false;
#if defined( _MSC_VER )
			std::array<int, 4> cpuInfo{};
			__cpuid( cpuInfo.data(), 1 );					// Basic features leaf
			hasSupport = ( cpuInfo[2] & ( 1 << 28 ) ) != 0; // ECX bit 28 = AVX
#elif defined( __GNUC__ )
			unsigned int eax, ebx, ecx, edx;
			if ( __get_cpuid( 1, &eax, &ebx, &ecx, &edx ) )
			{
				hasSupport = ( ecx & ( 1 << 28 ) ) != 0; // ECX bit 28 = AVX
			}
#endif
			return hasSupport;
		}();

		return s_hasAVX;
	}

	//----------------------------
	// AVX2 Detection
	//----------------------------

	inline bool hasAVX2Support() noexcept
	{
		static const bool s_hasAVX2 = []() {
			bool hasSupport = false;
#if defined( _MSC_VER )
			std::array<int, 4> cpuInfo{};
			__cpuid( cpuInfo.data(), 7 );				   // Extended features leaf
			hasSupport = ( cpuInfo[1] & ( 1 << 5 ) ) != 0; // EBX bit 5 = AVX2
#elif defined( __GNUC__ )
			unsigned int eax, ebx, ecx, edx;
			if ( __get_cpuid_count( 7, 0, &eax, &ebx, &ecx, &edx ) )
			{
				hasSupport = ( ebx & ( 1 << 5 ) ) != 0; // EBX bit 5 = AVX2
			}
#endif
			return hasSupport;
		}();

		return s_hasAVX2;
	}
} // namespace nfx::core::cpu
