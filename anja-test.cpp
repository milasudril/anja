//@	{"targets":[{"name":"anja-test","type":"application"}]}

#include "ui/window.hpp"
#include "ui/uicontext.hpp"
#include "ui/box.hpp"
#include "ui/paletteview.hpp"
#include "ui/colorview.hpp"
#include "ui/slider.hpp"
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
			ColorPicker(Container& cnt):
				m_box(cnt,true)
					,m_pal_view(m_box.insertMode(Box::InsertMode{4,0}))
					,m_hue(m_box,false)
						,m_hue_min(m_hue)
						,m_hue_slider(m_hue.insertMode(Box::InsertMode{2,Box::EXPAND|Box::FILL}),false)
						,m_hue_max(m_hue.insertMode(Box::InsertMode{0,0}))
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
				indexSelected(m_pal_view,0);
				}

			void indexSelected(PaletteView& palview,int id)
				{
				m_result.color(palview.color(palview.selection()));
				}

		private:
			Box m_box;
				PaletteView m_pal_view;
				Box m_hue;
					ColorView m_hue_min;
					Slider m_hue_slider;
					ColorView m_hue_max;
				Box m_sat;
					ColorView m_sat_min;
					Slider m_sat_slider;
					ColorView m_sat_max;
				Box m_lightness;
					ColorView m_lightness_min;
					Slider m_lightness_slider;	
					ColorView m_lightness_max;
				ColorView m_result;
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
