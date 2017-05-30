//@	{
//@	 "targets":[{"name":"clock.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"clock.o","rel":"implementation"}]
//@	}

#ifndef ANJA_CLOCK_HPP
#define ANJA_CLOCK_HPP

#include <cstdint>

namespace Anja
	{
	uint64_t now_ms();
	}

#endif
