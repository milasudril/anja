#ifdef __WAND__
target[name[color.h] type[include]]
dependency[color.o]
#endif

#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

struct ColorHSLA;
struct ColorHSVA;
struct ColorRGBA;

struct alignas(16) ColorRGBA
	{
	constexpr ColorRGBA():red(0),green(0),blue(0),alpha(1)
		{}

	constexpr ColorRGBA(float r,float g,float b,float a):
		red(r),green(g),blue(b),alpha(a)
		{}

	ColorRGBA(const ColorHSLA& hsla);
	ColorRGBA(const ColorHSVA& hsva);

	float red;
	float green;
	float blue;
	float alpha;
	};

constexpr ColorRGBA COLORS[]=
	{
	 {1.0f,1.0f,1.0f,1.0f}    //WHITE
	,{0.75f,0.75f,0.75f,1.0f} //GRAY
	,{0.25f,0.25f,0.25f,1.0f} //GRAY_DARK
	,{0.0f,0.0f,0.0f,1.0f}    //BLACK
	,{1.0f,0.0f,0.0f,1.0f}    //RED
	,{1.0f,0.5f,0.0f,1.0f}    //ORANGE
	,{1.0f,1.0f,0.0f,1.0f}    //YELLOW
	,{0.5f,0.1f,0.0f,1.0f}    //LIME
	,{0.0f,1.0f,0.0f,1.0f}    //GREEN
	,{0.0f,1.0f,0.5f,1.0f}    //TURQUOISE
	,{0.0f,1.0f,1.0f,1.0f}    //CYAN
	,{0.0f,0.5f,1.0f,1.0f}    //SKY
	,{0.0f,0.0f,1.0f,1.0f}    //BLUE
	,{0.5f,0.0f,1.0f,1.0f}    //VIOLETT
	,{1.0f,0.0f,1.0f,1.0f}    //MAGENTA
	,{1.0f,0.0f,0.5f,1.0f}    //PINK
	};

namespace ColorID
	{
	enum
		{
		 WHITE,GRAY,GRAY_DARK,BLACK
		,RED,ORANGE,YELLOW,LIME
		,GREEN,TURQOISE,CYAN,SKY
		,BLUE,VIOLETT,MAGNTA,PINK
		,COLOR_END
		};
	}

constexpr float luma709(const ColorRGBA& rgba)
	{
	return 0.2126f*rgba.red + 0.7152f*rgba.green + 0.0722f*rgba.blue;
	}

constexpr bool isDark(const ColorRGBA& rgba)
	{
	return luma709(rgba)<0.5f;
	}

constexpr bool isLight(const ColorRGBA& rgba)
	{return !isDark(rgba);}

struct alignas(16) ColorHSLA
	{
	constexpr ColorHSLA(float h,float s,float l,float a):
		hue(h),saturation(s),lightness(l),alpha(a)
		{}

	ColorHSLA(const ColorRGBA& rgba);
	static ColorHSLA fromHueSaturationLuma(float hue,float saturation,float luma_709);

	static ColorHSLA fromHueAndLuma(float hue,float luma_709)
		{return fromHueSaturationLuma(hue,1.0f,luma_709);}

	float hue;
	float saturation;
	float lightness;
	float alpha;
	};

struct alignas(16) ColorHSVA
	{
	constexpr ColorHSVA(float h,float s,float v,float a):
		hue(h),saturation(s),value(v),alpha(a)
		{}

	ColorHSVA(const ColorRGBA& rgba);

	float hue;
	float saturation;
	float value;
	float alpha;
	};



typedef ColorRGBA Color;

#endif
