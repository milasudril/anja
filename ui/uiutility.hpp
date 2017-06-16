//@	{
//@	 "targets":[{"name":"uiutility.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"uiutility.o","rel":"implementation"}]
//@	}

#ifndef ANJA_UIUTILITY_HPP
#define ANJA_UIUTILITY_HPP

#include "../common/color.hpp"

namespace Anja
	{
	ColorRGBA ambientColor(void* widget_handle);
	}

#endif