//@	{"targets":[{"name":"clock.o","type":"object"}]}


#include "clock.hpp"
#include <ctime>

using namespace Anja;

uint64_t Anja::now_ms()
	{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC_RAW,&now);
	return static_cast<uint64_t>(1000.0*(1e-9*now.tv_nsec+now.tv_sec ));
	}
