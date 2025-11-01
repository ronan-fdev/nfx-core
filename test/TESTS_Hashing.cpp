/**
 * @file TESTS_Hashing.cpp
 * @brief Comprehensive tests for hashing algorithms
 * @details Tests covering FNV-1a, CRC32, Larson, integer hashing, hash combining, and seed mixing
 */

#include <gtest/gtest.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include <nfx/core/CPU.h>
#include <nfx/core/Hashing.h>

namespace nfx::core::hashing::test
{
	using namespace nfx::core::hashing;
	using namespace nfx::core::hashing::constants;

	//=====================================================================
	// Low-Level hash functions
	//=====================================================================

	TEST( HashingBasic, LarsonHash )
	{
		// Larson hash: hash = 37 * hash + ch
		uint32_t hash{ 0 };

		hash = Larson( hash, 'A' );
		EXPECT_EQ( hash, 65u ); // 37 * 0 + 65

		hash = Larson( hash, 'B' );
		EXPECT_EQ( hash, 2471u ); // 37 * 65 + 66

		hash = Larson( hash, 'C' );
		EXPECT_EQ( hash, 91494u ); // 37 * 2471 + 67
	}

	TEST( HashingBasic, FNV1aHash )
	{
		// FNV-1a: hash = (hash ^ ch) * prime
		uint32_t hash{ DEFAULT_FNV_OFFSET_BASIS };

		hash = fnv1a<DEFAULT_FNV_PRIME>( hash, 'T' );
		EXPECT_NE( hash, DEFAULT_FNV_OFFSET_BASIS );

		uint32_t prevHash{ hash };
		hash = fnv1a<DEFAULT_FNV_PRIME>( hash, 'e' );
		EXPECT_NE( hash, prevHash );

		// Same input should produce same output
		uint32_t hash2{ DEFAULT_FNV_OFFSET_BASIS };
		hash2 = fnv1a<DEFAULT_FNV_PRIME>( hash2, 'T' );
		hash2 = fnv1a<DEFAULT_FNV_PRIME>( hash2, 'e' );
		EXPECT_EQ( hash, hash2 );
	}

	TEST( HashingBasic, CRC32Hash )
	{
		// CRC32 only works if SSE4.2 is available
		if ( !cpu::hasSSE42Support() )
		{
			GTEST_SKIP() << "SSE4.2 not available, skipping CRC32 tests";
		}

		uint32_t hash{ 0 };

		hash = crc32( hash, 'A' );
		EXPECT_NE( hash, 0u );

		uint32_t prevHash{ hash };
		hash = crc32( hash, 'B' );
		EXPECT_NE( hash, prevHash );

		// Same input should produce same output
		uint32_t hash2{ 0 };
		hash2 = crc32( hash2, 'A' );
		hash2 = crc32( hash2, 'B' );
		EXPECT_EQ( hash, hash2 );
	}

	//=====================================================================
	// String Hashing
	//=====================================================================

	TEST( HashingString, EmptyString )
	{
		uint32_t hash = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "" );

		// Empty string should return offset basis (FNV-1a behavior)
		EXPECT_EQ( hash, DEFAULT_FNV_OFFSET_BASIS );
	}

	TEST( HashingString, SimpleStrings )
	{
		auto hash1 = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "test" );
		auto hash2 = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "test" );
		auto hash3 = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "Test" );

		// Same input -> same output
		EXPECT_EQ( hash1, hash2 );

		// Different input -> different output (case sensitive)
		EXPECT_NE( hash1, hash3 );
	}

	TEST( HashingString, LongerStrings )
	{
		auto hash1 = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "The quick brown fox jumps over the lazy dog" );
		auto hash2 = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "The quick brown fox jumps over the lazy dog" );
		auto hash3 = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "The quick brown fox jumps over the lazy cat" );

		// Same input -> same output
		EXPECT_EQ( hash1, hash2 );

		// Different input -> different output
		EXPECT_NE( hash1, hash3 );
	}

	TEST( HashingString, StringViewVsStdString )
	{
		std::string str{ "consistency test" };
		std::string_view view{ str };

		auto hashFromView = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( view );
		auto hashFromString = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( str );

		// Should produce identical hashes
		EXPECT_EQ( hashFromView, hashFromString );
	}

	//=====================================================================
	// Integer hashing
	//=====================================================================

	TEST( HashingInteger, Int32Values )
	{
		// Test 32-bit integer hashing
		auto hash1 = hashInteger( int32_t{ 42 } );
		auto hash2 = hashInteger( int32_t{ 42 } );
		auto hash3 = hashInteger( int32_t{ 43 } );

		// Same input -> same output
		EXPECT_EQ( hash1, hash2 );

		// Different input -> different output
		EXPECT_NE( hash1, hash3 );
	}

	TEST( HashingInteger, Int64Values )
	{
		// Test 64-bit integer hashing
		auto hash1 = hashInteger( static_cast<int64_t>( 0xCAFEBABEDEADC0DEULL ) );
		auto hash2 = hashInteger( static_cast<int64_t>( 0xCAFEBABEDEADC0DEULL ) );
		auto hash3 = hashInteger( static_cast<int64_t>( 0xFEEDFACEDEADBEEFULL ) );

		// Same input -> same output
		EXPECT_EQ( hash1, hash2 );

		// Different input -> different output
		EXPECT_NE( hash1, hash3 );
	}

	TEST( HashingInteger, UnsignedValues )
	{
		auto hash1 = hashInteger( uint32_t{ 123456 } );
		auto hash2 = hashInteger( uint32_t{ 123456 } );
		auto hash3 = hashInteger( uint32_t{ 654321 } );

		EXPECT_EQ( hash1, hash2 );
		EXPECT_NE( hash1, hash3 );
	}

	TEST( HashingInteger, ZeroValue )
	{
		auto hashZero32 = hashInteger( int32_t{ 0 } );
		auto hashZero64 = hashInteger( int64_t{ 0 } );

		// Zero hashes to zero (multiplicative hashing property)
		EXPECT_EQ( hashZero32, 0u );
		EXPECT_EQ( hashZero64, 0u );

		// But non-zero values should hash differently
		auto hashOne32 = hashInteger( int32_t{ 1 } );
		auto hashOne64 = hashInteger( int64_t{ 1 } );
		EXPECT_NE( hashOne32, 0u );
		EXPECT_NE( hashOne64, 0u );
	}

	//=====================================================================
	// Hash combining
	//=====================================================================

	TEST( HashingCombine, Combine32Bit )
	{
		uint32_t hash1{ 0x12345678 };
		uint32_t hash2{ 0xABCDEF00 };

		uint32_t combined = combine( hash1, hash2, DEFAULT_FNV_PRIME );

		// Combined hash should be different from both inputs
		EXPECT_NE( combined, hash1 );
		EXPECT_NE( combined, hash2 );
		EXPECT_NE( combined, 0u );

		// Same inputs should produce same output
		uint32_t combined2 = combine( hash1, hash2, DEFAULT_FNV_PRIME );
		EXPECT_EQ( combined, combined2 );
	}

	TEST( HashingCombine, Combine64Bit )
	{
		size_t hash1{ 0xCAFEBABEDEADC0DE };
		size_t hash2{ 0xFEEDFACE12345678 };

		size_t combined = combine( hash1, hash2 );

		// Combined hash should be different from both inputs
		EXPECT_NE( combined, hash1 );
		EXPECT_NE( combined, hash2 );
		EXPECT_NE( combined, 0u );

		// Same inputs should produce same output
		size_t combined2 = combine( hash1, hash2 );
		EXPECT_EQ( combined, combined2 );
	}

	TEST( HashingCombine, MultipleValues )
	{
		// Combine multiple hashes sequentially
		uint32_t result{ 0 };

		result = combine( result, hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "name" ), DEFAULT_FNV_PRIME );
		result = combine( result, hashInteger( 42 ), DEFAULT_FNV_PRIME );
		result = combine( result, hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "value" ), DEFAULT_FNV_PRIME );

		EXPECT_NE( result, 0u );

		// Same sequence should produce same result
		uint32_t result2{ 0 };
		result2 = combine( result2, hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "name" ), DEFAULT_FNV_PRIME );
		result2 = combine( result2, hashInteger( 42 ), DEFAULT_FNV_PRIME );
		result2 = combine( result2, hashStringView<DEFAULT_FNV_OFFSET_BASIS>( "value" ), DEFAULT_FNV_PRIME );

		EXPECT_EQ( result, result2 );
	}

	//=====================================================================
	// Seed mixing
	//=====================================================================

	TEST( HashingSeedMix, BasicSeedMixing )
	{
		uint32_t seed{ 0x1A21DA };
		uint32_t hash{ 0xCAFEBABE };
		size_t tableSize{ 256 }; // Must be power of 2

		uint32_t index = seedMix( seed, hash, tableSize );

		// Index should be within table bounds
		EXPECT_LT( index, tableSize );

		// Same inputs -> same output
		uint32_t index2 = seedMix( seed, hash, tableSize );
		EXPECT_EQ( index, index2 );
	}

	TEST( HashingSeedMix, DifferentSeeds )
	{
		uint32_t hash{ 0xDEADC0DE };
		size_t tableSize{ 1024 };

		uint32_t index1 = seedMix( 0xCA7, hash, tableSize ); // "CAT"
		uint32_t index2 = seedMix( 0xD06, hash, tableSize ); // "DOG"

		// Note: Could theoretically collide, but very unlikely
		EXPECT_TRUE( index1 < tableSize );
		EXPECT_TRUE( index2 < tableSize );
	}

	//=====================================================================
	// Distribution Quality
	//=====================================================================

	TEST( HashingDistribution, StringDistribution )
	{
		// Generate hashes for many strings
		std::vector<std::string> testStrings;
		for ( int i = 0; i < 1000; ++i )
		{
			testStrings.push_back( "test_string_" + std::to_string( i ) );
		}

		std::unordered_set<uint32_t> uniqueHashes;
		for ( const auto& str : testStrings )
		{
			uint32_t hash = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( str );
			uniqueHashes.insert( hash );
		}

		// Should have no collisions - perfect distribution for this set
		EXPECT_EQ( uniqueHashes.size(), 1000u );
	}

	TEST( HashingDistribution, IntegerDistribution )
	{
		std::unordered_set<size_t> uniqueHashes;

		for ( int32_t i = 0; i < 1000; ++i )
		{
			size_t hash = hashInteger( i );
			uniqueHashes.insert( hash );
		}

		EXPECT_EQ( uniqueHashes.size(), 1000u );
	}

	//=====================================================================
	// Constexpr verification
	//=====================================================================

	TEST( HashingConstexpr, CompileTimeHashing )
	{
		// These should be evaluable at compile time
		constexpr uint32_t compileTimeHash = fnv1a<DEFAULT_FNV_PRIME>( DEFAULT_FNV_OFFSET_BASIS, 'X' );
		constexpr uint32_t compileTimeLarson = Larson( 0, 'Y' );
		constexpr uint32_t compileTimeCombine = combine( 0x12345678, 0xABCDEF00, DEFAULT_FNV_PRIME );
		constexpr uint32_t compileTimeSeedMix = seedMix( 0x1234, 0xABCD, 256 );

		// Runtime versions should match
		EXPECT_EQ( fnv1a<DEFAULT_FNV_PRIME>( DEFAULT_FNV_OFFSET_BASIS, 'X' ), compileTimeHash );
		EXPECT_EQ( Larson( 0, 'Y' ), compileTimeLarson );
		EXPECT_EQ( combine( 0x12345678, 0xABCDEF00, DEFAULT_FNV_PRIME ), compileTimeCombine );
		EXPECT_EQ( seedMix( 0x1234, 0xABCD, 256 ), compileTimeSeedMix );
	}

	//=====================================================================
	// Edge cases
	//=====================================================================

	TEST( HashingEdgeCases, VeryLongStrings )
	{
		std::string longString( 10000, 'X' );
		uint32_t hash = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( longString );

		EXPECT_NE( hash, DEFAULT_FNV_OFFSET_BASIS );

		// Same content should produce same hash
		std::string longString2( 10000, 'X' );
		uint32_t hash2 = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( longString2 );
		EXPECT_EQ( hash, hash2 );
	}

	TEST( HashingEdgeCases, SpecialCharacters )
	{
		std::string_view sv1( "\n\t\r\0test", 8 );
		std::string_view sv2( "\n\t\r\0test", 8 );

		auto hash1 = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( sv1 );
		auto hash2 = hashStringView<DEFAULT_FNV_OFFSET_BASIS>( sv2 );

		EXPECT_EQ( hash1, hash2 );
		EXPECT_NE( hash1, DEFAULT_FNV_OFFSET_BASIS );
	}

	TEST( HashingEdgeCases, ExtremIntegerValues )
	{
		auto hashMin32 = hashInteger( std::numeric_limits<int32_t>::min() );
		auto hashMax32 = hashInteger( std::numeric_limits<int32_t>::max() );
		auto hashMin64 = hashInteger( std::numeric_limits<int64_t>::min() );
		auto hashMax64 = hashInteger( std::numeric_limits<int64_t>::max() );

		// All should produce valid non-zero hashes
		EXPECT_NE( hashMin32, 0u );
		EXPECT_NE( hashMax32, 0u );
		EXPECT_NE( hashMin64, 0u );
		EXPECT_NE( hashMax64, 0u );

		// Should be different from each other
		EXPECT_NE( hashMin32, hashMax32 );
		EXPECT_NE( hashMin64, hashMax64 );
	}
} // namespace nfx::core::hashing::test
