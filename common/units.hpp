//@	{"targets":[{"name":"units.hpp","type":"include"}]}

#ifndef ANJA_UNITS_HPP
#define ANJA_UNITS_HPP

#include <cmath>
#include <cstdint>

namespace Anja
	{
	template<class T>
	inline T amplitudeToDb(const T& x)
		{return T(20)*log10(x);}

	template<class T>
	inline T powerToDb(const T& x)
		{return T(10)*log10(x);}

	template<class T>
	inline T dBtoPower(const T& x)
		{return pow(T(10),x/T(10));}

	template<class T>
	inline T dBToAmplitude(const T& x)
		{return pow(T(10),x/T(20));}

	inline constexpr double framesToSeconds(uint64_t n_frames,double fs)
		{return n_frames/fs;}

	inline constexpr uint64_t secondsToFrames(double seconds,double fs)
		{return uint64_t(seconds*fs+0.5);}

	inline double timeConstantToDecayFactor(double tau,double fs)
		{return exp(-1.0/(fs*tau));}
	}

#endif
