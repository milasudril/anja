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
	uint32_t data_offset;

	const uint8_t* pixelsGet() const
		{
		auto ptr=reinterpret_cast<const uint8_t*>(this + 1);
		return ptr+data_offset;
		}
	};

#endif
