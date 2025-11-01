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
 * @file Hashing.inl
 * @brief Implementation of core hash algorithms and infrastructure
 * @details Contains optimized hash functions using CRC32-C (Castagnoli) for strings with
 *          hardware acceleration (SSE4.2) when available, FNV-1a for legacy support,
 *          and multiplicative hashing for integers with proper avalanche properties
 */

namespace nfx::core::hashing
{
	//=====================================================================
	// Hash infrastructure
	//=====================================================================

	//----------------------------------------------
	// Low-level hash building blocks
	//----------------------------------------------

	inline constexpr uint32_t Larson( uint32_t hash, uint8_t ch ) noexcept
	{
		return 37 * hash + ch;
	}

	template <uint32_t FnvPrime>
	inline constexpr uint32_t fnv1a( uint32_t hash, uint8_t ch ) noexcept
	{
		hash ^= ch;		  // XOR byte into hash first
		hash *= FnvPrime; // Then multiply by prime

		return hash;
	}

	inline uint32_t crc32( uint32_t hash, uint8_t ch ) noexcept
	{
#if defined( _MSC_VER ) && !defined( __clang__ ) // Pure MSVC compiler
		return _mm_crc32_u8( hash, ch );
#elif defined( __SSE4_2__ ) // GCC, Clang, or Clang-CL with SSE4.2 support
		return __builtin_ia32_crc32qi( hash, ch );
#else
		// Software implementation of CRC32-C (Castagnoli) matching SSE4.2 _mm_crc32_u8
		// Polynomial: 0x1EDC6F41 (x^32 + x^28 + x^27 + x^26 + x^25 + x^23 + x^22 + ...)
		constexpr uint32_t polynomial = 0x82F63B78;

		uint32_t crc = hash ^ ch;
		for ( int i = 0; i < 8; ++i )
		{
			crc = ( crc >> 1 ) ^ ( ( crc & 1 ) ? polynomial : 0 );
		}

		return crc;
#endif
	}

	template <uint64_t MixConstant>
	inline constexpr uint32_t seedMix( uint32_t seed, uint32_t hash, size_t size ) noexcept
	{
		// Mixes the primary hash with the seed to find the final table slot
		uint32_t x{ seed + hash }; // Mix seed with original hash
		x ^= x >> 12;			   // Thomas Wang's bit-mixing: spread high bits to low positions
		x ^= x << 25;			   // Fold low bits back to high positions for avalanche effect
		x ^= x >> 27;			   // Final avalanche step ensures all bits influence result

		/*
		 * Final step: Multiplicative hashing with 64-bit magic constant followed by modulo reduction.
		 *
		 * - Multiply by MixConstant: A carefully chosen 64-bit odd constant that ensures
		 *   good bit distribution and avalanche properties. The multiplication spreads the mixed
		 *   bits of 'x' across the full 64-bit range, maximizing entropy.
		 *   Default: 0x2545F4914F6CDD1D (constants::DEFAULT_HASH_MIX_64)
		 *
		 * - Bitwise AND with (size - 1): Fast modulo operation that works because 'size' is
		 *   guaranteed to be a power of 2. This maps the large hash value to a valid table
		 *   index in range [0, size-1]. Equivalent to (x * constant) % size but much faster.
		 *
		 * - Cast to uint32_t: Return type matches the expected table index size.
		 */
		return static_cast<uint32_t>( ( x * MixConstant ) & ( size - 1 ) );
	}

	//----------------------------------------------
	// Hash combination
	//----------------------------------------------

	inline constexpr uint32_t combine( uint32_t existing, uint32_t newHash, uint32_t prime ) noexcept
	{
		// FNV-1a style combination: XOR then multiply
		existing ^= newHash;
		existing *= prime;

		return existing;
	}

	inline constexpr size_t combine( size_t existing, size_t newHash ) noexcept
	{
		existing ^= newHash + constants::DEFAULT_GOLDEN_RATIO_64 + ( existing << 6 ) + ( existing >> 2 );

		existing ^= existing >> 33;
		existing *= constants::DEFAULT_MURMUR3_C1;
		existing ^= existing >> 33;
		existing *= constants::DEFAULT_MURMUR3_C2;
		existing ^= existing >> 33;

		return existing;
	}

	//----------------------------------------------
	// High-level complete hash operations
	//----------------------------------------------

	//----------------------------
	// String hashing
	//----------------------------

	template <uint32_t InitialHash>
	inline uint32_t hashStringView( std::string_view key ) noexcept
	{
		if ( key.empty() )
		{
			return InitialHash;
		}

		uint32_t hashValue = InitialHash;

		for ( size_t i = 0; i < key.length(); ++i )
		{
			hashValue = crc32( hashValue, static_cast<uint8_t>( key[i] ) );
		}

		return hashValue;
	}

	//----------------------------
	// Integer hashing
	//----------------------------

	template <typename T>
	inline constexpr std::enable_if_t<std::is_integral_v<T>, size_t> hashInteger( T value ) noexcept
	{
		/*
		 * Integer hashing using avalanche mixing for uniform bit distribution.
		 *
		 * Purpose: Transform integer input into well-distributed hash values where small
		 * changes in input produce large, unpredictable changes in output (avalanche effect).
		 *
		 * 32-bit Algorithm (Knuth-style):
		 * - Mix high/low bits with XOR and right shift to spread bit dependencies
		 * - Multiply by carefully chosen odd constant for mathematical guarantees
		 * - Repeat mixing steps to ensure full avalanche across all bit positions
		 *
		 * 64-bit Algorithm (Wang's method):
		 * - Use different shift amounts (30, 27, 31) to mix bit regions
		 * - Apply two different multiplicative constants for maximum entropy
		 * - Each step ensures bits from different input regions influence output
		 *
		 * Both provide O(1) performance with excellent statistical properties.
		 */
		if constexpr ( sizeof( T ) <= 4 )
		{
			/* 32-bit integer hashing */
			uint32_t x = static_cast<uint32_t>( value );
			x = ( ( x >> 16 ) ^ x ) * constants::DEFAULT_INTEGER_HASH_32;
			x = ( ( x >> 16 ) ^ x ) * constants::DEFAULT_INTEGER_HASH_32;
			x = ( x >> 16 ) ^ x;
			return static_cast<size_t>( x );
		}
		else
		{
			/* 64-bit integer hashing */
			uint64_t x = static_cast<uint64_t>( value );
			x = ( x ^ ( x >> 30 ) ) * constants::DEFAULT_INTEGER_HASH_64_C1;
			x = ( x ^ ( x >> 27 ) ) * constants::DEFAULT_INTEGER_HASH_64_C2;
			x = x ^ ( x >> 31 );
			return static_cast<size_t>( x );
		}
	}
} // namespace nfx::core::hashing
