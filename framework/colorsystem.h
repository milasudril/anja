#ifdef __WAND__
target[name[colorsystem.h] type[include]]
#endif

#ifndef COLORSYSTEM_H
#define COLORSYSTEM_H

#include "color.h"

#include <cstddef>

template<class SystemType>
class ColorSystemCommon
	{
	public:
		constexpr ColorSystemCommon(SystemType value):m_value(value)
			{}

		constexpr ColorSystemCommon(const ColorSystemCommon& c)=default;
		ColorSystemCommon& operator=(const ColorSystemCommon<SystemType>& c)
			{
			m_value=c.m_value;
			return *this;
			}


		operator const SystemType*() const
			{return &m_value;}

	protected:
		SystemType m_value;
	};

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class ColorSystem:public ColorSystemCommon<COLORREF>
	{
	public:
		constexpr ColorSystem(const ColorRGBA& rgba):
			ColorSystemCommon(RGB(rgba.red/255,rgba.green/255,rgba.blue/255))
			{}
	};

#else
#include <gtk/gtk.h>
class ColorSystem:public ColorSystemCommon<GdkRGBA>
	{
	public:
		constexpr ColorSystem(const ColorRGBA& rgba):
			ColorSystemCommon({rgba.red,rgba.green,rgba.blue,rgba.alpha})
			{}
	};
#endif

#endif
