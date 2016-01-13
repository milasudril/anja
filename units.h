#ifdef __WAND__
target[name[units.h] type[include]]
#endif

#ifndef UNITS_H
#define UINTS_H

#include <cmath>
#include <cstdint>

template<class T>
T amplitudeToDb(const T& x)
	{return T(20)*log10(x);}

template<class T>
T powerToDb(const T& x)
	{return T(10)*log10(x);}

template<class T>
T dBtoPower(const T& x)
	{return pow(T(10),x/T(10));}

template<class T>
T dBToAmplitude(const T& x)
	{return pow(T(10),x/T(20));}

constexpr double framesToSeconds(uint64_t n_frames,double fs)
	{
	return n_frames/fs;
	}

constexpr uint64_t secondsToFrames(double seconds,double fs)
	{
	return uint64_t(seconds*fs+0.5);
	}

#endif
