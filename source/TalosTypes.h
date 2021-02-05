#pragma once

#include <cstdint>
#include <stdlib.h>

namespace Talos
{

    struct Point
    {
        float x, y;
    };

	struct ColorPoint
	{
		float r, g, b;
		float pos;
	};


	/** This class implements the xorshift128+ algorithm that is a very fast, top-quality 64-bit pseudo-random number generator.
	 * Its cycle length is 2^128-1, which is more than enough for any single-thread application.
	 * More details and algorithms can be found at http://xorshift.di.unimi.it/.
	 *
	 * Instances of RandomXS128 are not thread-safe.
     */
	class RandomXS128
	{
	protected:
		/** Normalization constant for double. */
		const double NORM_DOUBLE = 1.0 / (1LL << 53);

		/** Normalization constant for float. */
		const double NORM_FLOAT = 1.0 / (1LL << 24);

	public:
		/** Creates a new random number generator. This constructor sets the seed of the random number generator to a value very likely
         * to be distinct from any other invocation of this constructor.
         *
         * This implementation uses native random to generate the initial seed. */
		RandomXS128()
		{
			SetSeed(rand());
		}

		/** Creates a new random number generator using a single uint64_t seed.
		 * @param seed the initial seed */
		RandomXS128(uint64_t seed)
		{
			SetSeed(seed);
		}

		/** Creates a new random number generator using two uint64_t seeds.
		 * @param seed0 the first part of the initial seed
		 * @param seed1 the second part of the initial seed */
		RandomXS128(uint64_t seed0, uint64_t seed1)
		{
			SetState(seed0, seed1);
		}

		/** Returns the next pseudo-random, uniformly distributed uint64_t value from this random number generator's sequence.
		 *
		 * Subclasses should override this, as this is used by all other methods. */
		uint64_t NextLong()
		{
			uint64_t s1 = _seed0;
			uint64_t s0 = _seed1;
			_seed0 = s0;
			s1 ^= s1 << 23;
			return (_seed1 = (s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26))) + s0;
		}

		/** Returns the next pseudo-random, uniformly distributed int value from this random number generator's sequence.
		 *
		 * This implementation uses NextLong internally. */
		int NextInt()
		{
			return (int)NextLong();
		}

		/** Returns a pseudo-random, uniformly distributed int value between 0 (inclusive) and the specified value (exclusive),
		 * drawn from this random number generator's sequence.
		 *
		 * This implementation uses NextLong internally.
		 * @param n the positive bound on the random number to be returned.
		 * @return the next pseudo-random int value between 0 (inclusive) and n (exclusive). */
		uint32_t NextInt(uint32_t n)
		{
			return (uint32_t)NextLong(n);
		}

		/** Returns a pseudo-random, uniformly distributed {@code long} value between 0 (inclusive) and the specified value (exclusive),
		 * drawn from this random number generator's sequence. The algorithm used to generate the value guarantees that the result is
		 * uniform, provided that the sequence of 64-bit values produced by this generator is.
		 *
		 * This implementation uses NextLong internally.
		 * @param n the positive bound on the random number to be returned.
		 * @return the next pseudo-random uint64_t value between 0 (inclusive) and n (exclusive). */
		uint64_t NextLong(uint64_t n)
		{
			for (;;)
			{
				uint64_t bits = NextLong() >> 1;
				uint64_t value = bits % n;
				if (bits - value + (n - 1) >= 0) return value;
			}
		}

		/** Returns a pseudo-random, uniformly distributed double value between 0.0 and 1.0 from this random number generator's sequence.
		 * 
		 * This implementation uses NextLong internally. */
		double NextDouble()
		{
			return (NextLong() >> 11) * NORM_DOUBLE;
		}

		/** Returns a pseudo-random, uniformly distributed float value between 0.0 and 1.0 from this random number generator's
		 * sequence.
		 * 
		 * This implementation uses NextLong internally. */
		float NextFloat()
		{
			return (float)((NextLong() >> 40)* NORM_FLOAT);
		}

		/** Returns a pseudo-random, uniformly distributed boolean value from this random number generator's sequence.
		 * 
		 * This implementation uses NextLong internally. */
		bool NextBoolean()
		{
			return (NextLong() & 1) != 0;
		}

		/** Generates random bytes and places them into a user-supplied byte array. The number of random bytes produced is equal to the
		 * length of the byte array.
		 *
		 * This implementation uses NextLong internally. */
		void NextBytes(char* bytes, int length)
		{
			int n = 0;
			int i = length;
			while (i != 0)
			{
				n = i < 8 ? i : 8; // min(i, 8);
				for (uint64_t bits = NextLong(); n-- != 0; bits >>= 8)
					bytes[--i] = (char)bits;
			}
		}

		/** Sets the internal seed of this generator based on the given uint64_t value.
		 *
		 * The given seed is passed twice through a hash function. This way, if the user passes a small value we avoid the short
		 * irregular transient associated with states having a very small number of bits set.
		 * @param seed a nonzero seed for this generator (if zero, the generator will be seeded with UINT64_MAX). */
		void SetSeed(uint64_t seed)
		{
			uint64_t seed0 = MurmurHash3(seed == 0 ? UINT64_MAX : seed);
			SetState(seed0, MurmurHash3(seed0));
		}

		/** Sets the internal state of this generator.
		 * @param seed0 the first part of the internal state
		 * @param seed1 the second part of the internal state */
		void SetState(uint64_t seed0, uint64_t seed1)
		{
			_seed0 = seed0;
			_seed1 = seed1;
		}

		/**
		 * Returns the internal seeds to allow state saving.
		 * @param seed must be 0 or 1, designating which of the 2 long seeds to return
		 * @return the internal seed that can be used in SetState
		 */
		uint64_t GetState(int seed)
		{
			return seed == 0 ? _seed0 : _seed1;
		}

		static uint64_t MurmurHash3(uint64_t x) {
			x ^= x >> 33;
			x *= 0xff51afd7ed558ccdL;
			x ^= x >> 33;
			x *= 0xc4ceb9fe1a85ec53L;
			x ^= x >> 33;

			return x;
		}
		
	protected:
		/** The first half of the internal state of this pseudo-random number generator. */
		uint64_t _seed0 = 0;

		/** The second half of the internal state of this pseudo-random number generator. */
		uint64_t _seed1 = 0;
	};
	
}
