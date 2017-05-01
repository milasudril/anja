//@	{
//@	 "targets":[{"name":"keymask.hpp","type":"include"}]
//@	,"dpendencies_extra":[{"ref":"keymask.o","rel":"implementation"}]
//@	}

#ifndef ANJA_KEYMASK_HPP
#define ANJA_KEYMASK_HPP

#include <cstdint>

namespace Anja
	{
	typedef uint16_t keymask_t;

	constexpr keymask_t KEYMASK_MOUSE_LEFT=0x0001;
	constexpr keymask_t KEYMASK_MOUSE_MID=0x0002;
	constexpr keymask_t KEYMASK_MOUSE_RIGHT=0x0004;
	constexpr keymask_t KEYMASK_MOUSE_ALL=0x00ff;

	constexpr keymask_t KEYMASK_KEY_SHIFT=0x0100;
	constexpr keymask_t KEYMASK_KEY_CTRL=0x0200;
	constexpr keymask_t KEYMASK_KEY_ALL=0xff00;

	keymask_t keymaskFromSystem(uint32_t mask_system);
	}

#endif
