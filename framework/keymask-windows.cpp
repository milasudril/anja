#ifdef __WAND__
target[name[keymask.o] type[object] platform[;Windows]]
#endif

#include "keymask.h"
#include <windows.h>

keymask_t keymaskFromSystem(uint32_t mask_system)
	{
	uint16_t ret=0;
	if(mask_system&MK_SHIFT)
		{ret|=KEYMASK_KEY_SHIFT;}
	if(mask_system&MK_LBUTTON)
		{ret|=KEYMASK_KEY_CTRL;}
	if(mask_system&MK_LBUTTON)
		{ret|=KEYMASK_MOUSE_LEFT;}
	if(mask_system&MK_MBUTTON)
		{ret|=KEYMASK_MOUSE_MID;}
	if(mask_system&MK_RBUTTON)
		{ret|=KEYMASK_MOUSE_RIGHT;}
	return ret;
	}
