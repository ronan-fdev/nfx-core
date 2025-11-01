/**
 * @file Sample_CPU.cpp
 * @brief Demonstrates CPU feature detection capabilities
 * @details This sample shows how to detect SSE4.2, AVX, and AVX2 support
 *          for runtime algorithm selection and optimization
 */

#include <iostream>
#include <iomanip>

#include <nfx/core/CPU.h>

int main()
{
	std::cout << "=== nfx-core - CPU Feature Detection ===" << std::endl;
	std::cout << std::endl;

	//=========================================================================
	// CPU capability detection
	//=========================================================================

	std::cout << "--- Detected CPU Capabilities ---" << std::endl;
	std::cout << std::endl;

	// SSE4.2 detection
	const bool hasSSE42 = nfx::core::cpu::hasSSE42Support();
	std::cout << "  SSE4.2 (Streaming SIMD Extensions 4.2):" << std::endl;
	std::cout << "    Status: " << ( hasSSE42 ? "SUPPORTED" : "NOT SUPPORTED" ) << std::endl;
	std::cout << "    Features: CRC32 hardware acceleration" << std::endl;
	std::cout << "    Impact: " << ( hasSSE42 ? "3-5x faster hashing with CRC32 intrinsics" : "Using FNV-1a software fallback" ) << std::endl;
	std::cout << std::endl;

	// AVX detection
	const bool hasAVX = nfx::core::cpu::hasAVXSupport();
	std::cout << "  AVX (Advanced Vector Extensions):" << std::endl;
	std::cout << "    Status: " << ( hasAVX ? "SUPPORTED" : "NOT SUPPORTED" ) << std::endl;
	std::cout << "    Features: 256-bit floating-point SIMD operations" << std::endl;
	std::cout << "    Impact: " << ( hasAVX ? "Vectorized FP math available" : "Limited to 128-bit SSE" ) << std::endl;
	std::cout << std::endl;

	// AVX2 detection
	const bool hasAVX2 = nfx::core::cpu::hasAVX2Support();
	std::cout << "  AVX2 (Advanced Vector Extensions 2):" << std::endl;
	std::cout << "    Status: " << ( hasAVX2 ? "SUPPORTED" : "NOT SUPPORTED" ) << std::endl;
	std::cout << "    Features: 256-bit integer SIMD operations" << std::endl;
	std::cout << "    Impact: " << ( hasAVX2 ? "Parallel hash computation possible" : "Sequential processing only" ) << std::endl;
	std::cout << std::endl;

	//=========================================================================
	// Feature summary
	//=========================================================================

	std::cout << "--- Feature Summary ---" << std::endl;
	std::cout << std::endl;

	int supportedFeatures = 0;
	if ( hasSSE42 )
	{
		supportedFeatures++;
	}
	if ( hasAVX )
	{
		supportedFeatures++;
	}
	if ( hasAVX2 )
	{
		supportedFeatures++;
	}

	std::cout << "  Total features detected: " << supportedFeatures << " / 3" << std::endl;
	std::cout << std::endl;

	if ( supportedFeatures == 3 )
	{
		std::cout << "  Excellent! Your CPU supports all modern SIMD extensions." << std::endl;
		std::cout << "  nfx-core will use the fastest available algorithms." << std::endl;
	}
	else if ( supportedFeatures >= 1 )
	{
		std::cout << "  Good! Your CPU has partial SIMD support." << std::endl;
		std::cout << "  nfx-core will use hardware acceleration where available." << std::endl;
	}
	else
	{
		std::cout << "  Warning: Your CPU doesn't support advanced SIMD extensions." << std::endl;
		std::cout << "  nfx-core will use optimized software fallbacks." << std::endl;
	}
	std::cout << std::endl;

	//=========================================================================
	// Algorithm selection demo
	//=========================================================================

	std::cout << "--- Automatic Algorithm Selection ---" << std::endl;
	std::cout << std::endl;

	std::cout << "  For string hashing, nfx-core will use:" << std::endl;
	if ( hasSSE42 )
	{
		std::cout << "    → CRC32 with SSE4.2 hardware intrinsics" << std::endl;
		std::cout << "    → Expected performance: 3-5x faster than software" << std::endl;
	}
	else
	{
		std::cout << "    → FNV-1a software implementation" << std::endl;
		std::cout << "    → Expected performance: Standard (no acceleration)" << std::endl;
	}
	std::cout << std::endl;

	return 0;
}
