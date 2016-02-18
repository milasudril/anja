#ifdef __WAND__
target[name[rgbablock.h] type[include]]
#endif

#ifndef RGBABLOCK_H
#define RGBABLOCK_H

struct RGBABlock
	{
	union
		{
		char string[4];
		uint32_t number;
		} magic;

	uint32_t width;
	uint32_t height;
	uint32_t reserved;
	uint8_t data[1];
	};

#endif
