//@	{"targets":[{"name":"focussink.hpp","type":"include"}]}

#ifndef ANJA_FOCUSSINK_HPP
#define ANJA_FOCUSSINK_HPP

namespace Anja
	{
	struct FocusSink
		{
		void* object;
		void (*action)(void* obj);
		};
	}

#endif
