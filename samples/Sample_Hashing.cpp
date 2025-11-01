/**
 * @file Sample_Hashing.cpp
 * @brief Demonstrates hashing utilities for strings, integers, and hash combining
 * @details This sample shows how to use FNV-1a, CRC32, Larson hashing, integer
 *          hashing, hash combining, and seed mixing for hash tables and algorithms
 */

#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <nfx/core/Hashing.h>

int main()
{
	std::cout << "=== nfx-core Hashing Utilities ===" << std::endl;
	std::cout << std::endl;

	//=========================================================================
	// Basic string hashing - FNV-1a algorithm
	//=========================================================================

	std::cout << "--- String Hashing (FNV-1a) ---" << std::endl;

	const std::string_view testStrings[]{
		"hello",
		"world",
		"FNV-1a is fast!",
		"",
		"The quick brown fox jumps over the lazy dog" };

	for ( const auto& str : testStrings )
	{
		using namespace nfx::core::hashing;
		const uint32_t hash{ hashStringView<constants::DEFAULT_FNV_OFFSET_BASIS>( str ) };

		std::cout << "  \"" << std::setw( 45 ) << str << "\" -> 0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << hash << std::dec << std::setfill( ' ' ) << std::endl;
	}

	std::cout << std::endl;

	//=========================================================================
	// Integer hashing - Fast mixing for hash tables
	//=========================================================================

	std::cout << "--- Integer Hashing (Murmur-style mixing) ---" << std::endl;

	const uint64_t testIntegers[]{ 0, 1, 42, 1000000, 0xCAFEBABE, 0xDEADC0DE, 0xFEEDFACE, 0xFFFFFFFFFFFFFFFF };

	for ( const auto value : testIntegers )
	{
		using namespace nfx::core::hashing;
		const uint64_t hash{ hashInteger( value ) };

		std::cout << "  0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << value << " -> 0x" << std::setw( 16 ) << hash << std::dec << std::setfill( ' ' ) << std::endl;
	}

	std::cout << std::endl;

	//=========================================================================
	// Hash combining - For composite keys
	//=========================================================================

	std::cout << "--- Hash Combining (Composite Keys) ---" << std::endl;

	struct Person
	{
		std::string firstName;
		std::string lastName;
		int age;

		[[nodiscard]] size_t hash() const
		{
			using namespace nfx::core::hashing;
			size_t h{ hashStringView<constants::DEFAULT_FNV_OFFSET_BASIS>( firstName ) };
			h = combine( h, hashStringView<constants::DEFAULT_FNV_OFFSET_BASIS>( lastName ) );
			h = combine( h, hashInteger( age ) );
			return h;
		}
	};

	const Person people[]{
		{ "John", "Doe", 30 },
		{ "Jane", "Smith", 25 },
		{ "John", "Smith", 30 }, // Different person, different hash
		{ "Jane", "Doe", 25 }	 // Different person, different hash
	};

	for ( const auto& person : people )
	{
		const size_t personHash{ person.hash() };
		std::cout << "  " << std::setw( 12 ) << person.firstName << " " << std::setw( 10 ) << person.lastName << " (age " << std::setw( 2 ) << person.age << ") -> 0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << personHash << std::dec << std::setfill( ' ' ) << std::endl;
	}

	std::cout << std::endl;

	//=========================================================================
	// Custom hash table with FNV-1a
	//=========================================================================

	std::cout << "--- Custom Hash Table Example ---" << std::endl;

	struct StringViewHash
	{
		[[nodiscard]] size_t operator()( const std::string_view sv ) const noexcept
		{
			return nfx::core::hashing::hashStringView<nfx::core::hashing::constants::DEFAULT_FNV_OFFSET_BASIS>( sv );
		}
	};

	std::unordered_map<std::string_view, int, StringViewHash> wordCounts;

	const std::string text{ "the quick brown fox jumps over the lazy dog the fox is quick" };
	std::cout << "Text: \"" << text << "\"" << std::endl;
	std::cout << std::endl;

	// Simple word counting using string_view
	size_t startPos{};
	for ( size_t i{}; i <= text.size(); ++i )
	{
		if ( i == text.size() || text.at( i ) == ' ' )
		{
			if ( i > startPos )
			{
				std::string_view word{ text.data() + startPos, i - startPos };
				wordCounts[word]++;
			}
			startPos = i + 1;
		}
	}

	std::cout << "Word Counts (using FNV-1a hash):" << std::endl;
	for ( const auto& [word, count] : wordCounts )
	{
		std::cout << "  " << std::setw( 10 ) << word << ": " << count << std::endl;
	}

	std::cout << std::endl;

	//=========================================================================
	// Seed mixing for hash table probing
	//=========================================================================

	std::cout << "--- Seed Mixing (CHD/Hash Table Probing) ---" << std::endl;

	const uint64_t testSeeds[]{ 0x0, 0x1, 0x42, 0x1A21DA, 0xCA7, 0xD06 };

	std::cout << "Mixed seeds for hash table probing:" << std::endl;
	for ( const auto seed : testSeeds )
	{
		using namespace nfx::core::hashing;
		const uint32_t mixed{ seedMix( seed, 0xABCDEF01, 256 ) }; // Example with hash and table size

		std::cout << "  seed=0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << seed << " -> mixed=0x" << std::setw( 16 ) << mixed << std::dec << std::setfill( ' ' ) << std::endl;
	}

	std::cout << std::endl;

	//=========================================================================
	// Low-level building blocks
	//=========================================================================

	std::cout << "--- Low-Level Building Blocks ---" << std::endl;

	// Manual FNV-1a construction
	using namespace nfx::core::hashing::constants;

	std::cout << "FNV-1a Constants:" << std::endl;
	std::cout << "  Basis:  0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << DEFAULT_FNV_OFFSET_BASIS << std::dec << std::setfill( ' ' ) << std::endl;
	std::cout << "  Prime:  0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << DEFAULT_FNV_PRIME << std::dec << std::setfill( ' ' ) << std::endl;
	std::cout << std::endl;

	// Manual hash construction
	std::string_view manualInput{ "test" };
	std::cout << "Manual FNV-1a hash of \"" << manualInput << "\":" << std::endl;

	uint32_t manualHash{ DEFAULT_FNV_OFFSET_BASIS };
	std::cout << "  Start:   0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << manualHash << std::dec << std::setfill( ' ' ) << std::endl;

	for ( const auto ch : manualInput )
	{
		manualHash = nfx::core::hashing::fnv1a<DEFAULT_FNV_PRIME>( manualHash, static_cast<uint8_t>( ch ) );
		std::cout << "    '" << ch << "' -> 0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << manualHash << std::dec << std::setfill( ' ' ) << std::endl;
	}

	std::cout << "  Result:  0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << manualHash << std::dec << std::setfill( ' ' ) << std::endl;

	// Verify against library function
	const uint32_t libraryHash{ nfx::core::hashing::hashStringView<DEFAULT_FNV_OFFSET_BASIS>( manualInput ) };
	std::cout << "  Library: 0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << libraryHash << std::dec << std::setfill( ' ' ) << std::endl;
	std::cout << "  Match:   " << ( manualHash == libraryHash ? "YES" : "NO" ) << std::endl;

	std::cout << std::endl;

	//=========================================================================
	// Performance comparison
	//=========================================================================

	std::cout << "--- Performance Comparison ---" << std::endl;

	// Generate test data
	std::vector<std::string> testData;
	testData.reserve( 10000 );
	for ( int i{}; i < 10000; ++i )
	{
		testData.push_back( "test_string_" + std::to_string( i ) );
	}

	// Benchmark FNV-1a hashing
	auto startTime{ std::chrono::high_resolution_clock::now() };

	uint64_t hashSum{}; // Prevent optimization
	for ( const auto& str : testData )
	{
		hashSum += nfx::core::hashing::hashStringView<nfx::core::hashing::constants::DEFAULT_FNV_OFFSET_BASIS>( str );
	}

	auto endTime{ std::chrono::high_resolution_clock::now() };
	auto duration{ std::chrono::duration_cast<std::chrono::microseconds>( endTime - startTime ) };

	std::cout << "Hashed " << testData.size() << " strings" << std::endl;
	std::cout << "Time:   " << duration.count() << " microseconds" << std::endl;
	std::cout << "Rate:   " << ( static_cast<double>( testData.size() ) * 1000000.0 / static_cast<double>( duration.count() ) ) << " strings/second" << std::endl;
	std::cout << "Sum:    0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << hashSum << std::dec << std::setfill( ' ' ) << " (prevent optimization)" << std::endl;

	std::cout << std::endl;

	//=========================================================================
	// Hash distribution analysis
	//=========================================================================

	std::cout << "--- Hash Distribution Analysis ---" << std::endl;

	// Test hash distribution with similar strings
	const std::string_view similarStrings[]{
		"user_001",
		"user_002",
		"user_003",
		"user_010",
		"user_100",
		"User_001", // Case variation
		"user_0010" // Length variation
	};

	std::cout << "Similar strings (testing avalanche effect):" << std::endl;
	for ( const auto& str : similarStrings )
	{
		const uint32_t hash{ nfx::core::hashing::hashStringView<nfx::core::hashing::constants::DEFAULT_FNV_OFFSET_BASIS>( str ) };
		std::cout << "  \"" << std::setw( 10 ) << str << "\" -> 0x" << std::hex << std::setw( 16 ) << std::setfill( '0' ) << hash << std::dec << std::setfill( ' ' ) << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Note: Small input changes produce very different hash values (avalanche effect)" << std::endl;

	std::cout << std::endl;

	//=========================================================================
	// Practical use case: fast string deduplication
	//=========================================================================

	std::cout << "--- Practical Use Case: String Deduplication ---" << std::endl;

	const std::string_view duplicatedStrings[]{
		"apple", "banana", "cherry", "apple", "banana", "date", "apple", "elderberry", "cherry", "banana" };

	std::unordered_map<std::string_view, size_t, StringViewHash> uniqueStrings;

	std::cout << "Input strings: ";
	for ( size_t i{}; i < std::size( duplicatedStrings ); ++i )
	{
		if ( i > 0 )
		{
			std::cout << ", ";
		}
		std::cout << duplicatedStrings[i];
	}
	std::cout << std::endl;

	for ( const auto& str : duplicatedStrings )
	{
		uniqueStrings[str]++;
	}

	std::cout << "Unique strings found: " << uniqueStrings.size() << std::endl;
	std::cout << "Frequency count:" << std::endl;
	for ( const auto& [str, count] : uniqueStrings )
	{
		std::cout << "  " << std::setw( 12 ) << str << ": " << count << " occurrences" << std::endl;
	}

	return 0;
}
