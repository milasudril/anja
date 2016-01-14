#ifdef __WAND__
target[name[randomgeneratorimpl.h] type[include]]
#endif

#include "randomgenerator.h"

#ifndef RANDOMGENERATORIMPL_H
#define RANDOMGENERATORIMPL_H

#include <random>

template<class RandomEngine>
class RandomGeneratorImpl:public RandomGenerator
	{
	public:
		RandomGeneratorImpl(){}
		~RandomGeneratorImpl(){}

		void seed(uintmax_t x) noexcept
			{m_rng.seed(x);}

		uintmax_t operator()() noexcept
			{return m_rng();}

		void discard(unsigned long long z) noexcept
			{m_rng.discard(z);}

		uintmax_t min() const noexcept
			{return m_rng.min();}

		uintmax_t max() const noexcept
			{return m_rng.max();}

		float drawUniform(float a) noexcept
			{
			if(a<1e-6f)
				{return 0.0f;}
			std::uniform_real_distribution<float> U(-a,a);
			return U(m_rng);
			}

	private:
		RandomEngine m_rng;
	};

#endif
