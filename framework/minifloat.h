#ifdef __WAND__
target[name[minifloat.h] type[include]]
dependency[minifloat.o]
#endif

#ifndef MINIFLOAT_H
#define MINIFLOAT_H

#include <cstdint>

#include <cstdlib>
#include <cstdio>

class Minifloat
	{
	public:

		constexpr Minifloat():m_bits(0){}

		operator double() const;

		static constexpr Minifloat fromBits(uint8_t bits)
			{
			return Minifloat{bits};
			}

	private:
		constexpr Minifloat(uint8_t bits):m_bits(bits){}

		uint8_t m_bits;
	};

#endif
