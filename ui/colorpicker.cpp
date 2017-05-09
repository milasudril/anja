//@	{
//@	 "targets":[{"name":"colorpicker.o","type":"object"}]
//@	}

#include "colorpicker.hpp"
#include "../common/colorstring.hpp"

using namespace Anja;

ColorPicker::ColorPicker(Container& cnt):
	m_box(cnt,true)
		,m_pal_view(m_box.insertMode(Box::InsertMode{4,0}))
		,m_hue(m_box,false)
			,m_hue_slider(m_hue.insertMode(Box::InsertMode{2,Box::EXPAND|Box::FILL}),false)
		,m_sat(m_box,false)
			,m_sat_min(m_sat)
			,m_sat_slider(m_sat.insertMode(Box::InsertMode{2,Box::EXPAND|Box::FILL}),false)
			,m_sat_max(m_sat.insertMode(Box::InsertMode{0,0}))
		,m_lightness(m_box,false)
			,m_lightness_min(m_lightness)
			,m_lightness_slider(m_lightness.insertMode(Box::InsertMode{2,Box::EXPAND|Box::FILL}),false)
			,m_lightness_max(m_lightness.insertMode(Box::InsertMode{0,0}))
		,m_result(m_box.insertMode(Box::InsertMode{4,Box::EXPAND|Box::FILL}))
		,m_rgb(m_box)
	{
	m_pal_view.callback(*this,0);
	m_hue_slider.callback(*this,SliderId::HUE);
	m_sat_slider.callback(*this,SliderId::SATURATION);
	m_lightness_slider.callback(*this,SliderId::LIGHTNESS);
	m_rgb.callback(*this,0);
	indexSelected(m_pal_view,0);
	m_lightness_min.color(ColorRGBA(0.0f,0.0f,0.0f,1.0f));
	m_lightness_max.color(ColorRGBA(1.0f,1.0f,1.0f,1.0f));
	color(ColorRGBA(ColorHSLA(0.5f,0.5f,0.5f,1.0f)));
	}

void ColorPicker::indexSelected(PaletteView& palview,int id)
	{color(palview.color(palview.selection()));}

void ColorPicker::changed(Slider& slider,SliderId id)
	{
	switch(id)
		{
		case SliderId::HUE:
			viewUpdate(ColorRGBA(ColorHSLA(slider.value(),m_sat_slider.value(),m_lightness_slider.value(),1.0f)));
			break;
		case SliderId::SATURATION:
			slider.value(std::max(slider.value(),1e-7));
			viewUpdate(ColorRGBA(ColorHSLA(m_hue_slider.value(),slider.value(),m_lightness_slider.value(),1.0f)));
			break;
		case SliderId::LIGHTNESS:
			slider.value(std::max(slider.value(),1e-7));
			viewUpdate(ColorRGBA(ColorHSLA(m_hue_slider.value(),m_sat_slider.value(),slider.value(),1.0f)));
			break;
		}
	}

void ColorPicker::viewUpdate(const ColorRGBA& color)
	{
	m_result.color(color);
	m_pal_view.color(color,m_pal_view.selection());
	m_rgb.content(ColorString(color).begin());
	ColorHSLA hsla(color);
	hsla.saturation=0.0f;
	m_sat_min.color(ColorRGBA(hsla));
	hsla.saturation=1.0f;
	m_sat_max.color(ColorRGBA(hsla));
	}

void ColorPicker::slidersUpdate(const ColorRGBA& color)
	{
	ColorHSLA hsla(color);
	m_hue_slider.value(hsla.hue);
	m_sat_slider.value(std::max(hsla.saturation,1e-7f));
	m_lightness_slider.value(std::max(hsla.lightness,1e-7f));
	}

void ColorPicker::changed(TextEntry& entry,int id) noexcept
	{
	ColorRGBA c;
	if(colorFromString(entry.content(),c))
		{color(c);}
	else
		{viewUpdate(m_result.color());}
	}
