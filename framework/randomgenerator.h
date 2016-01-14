#ifdef __WAND__
target[name[randomgenerator.h] type[include]]
#endif

#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <cstdint>

class RandomGenerator
	{
	public:
		virtual ~RandomGenerator()=default;
		virtual void seed(uintmax_t x) noexcept=0;
		virtual uintmax_t operator()() noexcept=0;
		virtual void discard(unsigned long long z) noexcept=0;
		virtual uintmax_t min() const noexcept=0;
		virtual uintmax_t max() const noexcept=0;
		virtual float drawUniform(float a) noexcept=0;
	};

#endif
