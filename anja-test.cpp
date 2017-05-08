//@	{"targets":[{"name":"anja-test","type":"application"}]}

#include "ui/window.hpp"
#include "ui/uicontext.hpp"
#include "ui/box.hpp"
#include "ui/paletteview.hpp"
#include "ui/colorview.hpp"
#include "ui/slider.hpp"
#include "common/color.hpp"
#include <cstdio>

namespace
	{
	class SessionControl
		{
		public:
			explicit SessionControl(Anja::UiContext& ctx):r_ctx(ctx)
				{}

			void closing(Anja::Window& win,int id)
				{r_ctx.exit();}

			Anja::UiContext::RunStatus idle(Anja::UiContext& ctx)
				{
				return Anja::UiContext::RunStatus::WAIT;
				}

		private:
			Anja::UiContext& r_ctx;
		};
	}

namespace Anja
	{
	class ColorPicker
		{
		public:
			enum class SliderId:int{HUE,SATURATION,LIGHTNESS};

			ColorPicker(Container& cnt):
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
				{
				m_pal_view.callback(*this,0);
				m_hue_slider.callback(*this,SliderId::HUE);
				m_sat_slider.callback(*this,SliderId::SATURATION);
				m_lightness_slider.callback(*this,SliderId::LIGHTNESS);
				indexSelected(m_pal_view,0);
				m_lightness_min.color(ColorRGBA(0.0f,0.0f,0.0f,1.0f));
				m_lightness_max.color(ColorRGBA(1.0f,1.0f,1.0f,1.0f));
				color(ColorRGBA(ColorHSLA(0.5f,0.5f,0.5f,1.0f)));
				}

			void indexSelected(PaletteView& palview,int id)
				{
				color(palview.color(palview.selection()));
				}

			void changed(Slider& slider,SliderId id)
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

		/*	const ColorRGBA& color() noexcept
				{return m_result.color();}*/

			ColorPicker& color(const ColorRGBA& color)
				{
				slidersUpdate(color);
				return *this;
				}

			

		private:
			Box m_box;
				PaletteView m_pal_view;
				Box m_hue;
					Slider m_hue_slider;
				Box m_sat;
					ColorView m_sat_min;
					Slider m_sat_slider;
					ColorView m_sat_max;
				Box m_lightness;
					ColorView m_lightness_min;
					Slider m_lightness_slider;	
					ColorView m_lightness_max;
				ColorView m_result;

			void viewUpdate(const ColorRGBA& color)
				{
				m_result.color(color);
				m_pal_view.color(color,m_pal_view.selection());
				ColorHSLA hsla(color);
				hsla.saturation=0.0f;
				m_sat_min.color(ColorRGBA(hsla));
				hsla.saturation=1.0f;
				m_sat_max.color(ColorRGBA(hsla));
				}

			void slidersUpdate(const ColorRGBA& color)
				{
				ColorHSLA hsla(color);
				m_hue_slider.value(hsla.hue);
				m_sat_slider.value(std::max(hsla.saturation,1e-7f));
				m_lightness_slider.value(std::max(hsla.lightness,1e-7f));
				}
		};
	};

int main(int argc, char **argv)
	{
	try
		{
		Anja::UiContext ctx;
		ctx.dark(1);
		SessionControl ctrl(ctx);
		Anja::Window mainwin("Test");
		mainwin.callback(ctrl,0);
		Anja::ColorPicker picker(mainwin);
		mainwin.show();
		ctx.run(ctrl);
		}
	catch(const char* err)
		{
		fprintf(stderr,"Error: %s\n",err);
		return -1;
		}
	return 0;
	}
