//@	 {"targets":[{"name":"keymask.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "keymask.hpp"
#include <gtk/gtk.h>

using namespace Anja;

keymask_t Anja::keymaskFromSystem(uint32_t mask_system)
	{
	uint16_t ret=0;
	if(mask_system&GDK_SHIFT_MASK)
		{ret|=KEYMASK_KEY_SHIFT;}
	if(mask_system&GDK_CONTROL_MASK)
		{ret|=KEYMASK_KEY_CTRL;}
	if(mask_system&GDK_BUTTON1_MASK)
		{ret|=KEYMASK_MOUSE_LEFT;}
	if(mask_system&GDK_BUTTON2_MASK)
		{ret|=KEYMASK_MOUSE_MID;}
	if(mask_system&GDK_BUTTON3_MASK)
		{ret|=KEYMASK_MOUSE_RIGHT;}
	return ret;
	}
