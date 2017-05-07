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
					,m_pal_view(m_box)
					,m_hue(m_box,false)
						,m_hue_min(m_hue)
						,m_hue_slider(m_hue.insertMode(Box::InsertMode{2,Box::EXPAND|Box::FILL}),false)
					,m_sat(m_box,false)
						,m_sat_slider(m_sat.insertMode(Box::InsertMode{2,Box::EXPAND|Box::FILL}),false)
					,m_lightness(m_box,false)
						,m_lightness_slider(m_lightness.insertMode(Box::InsertMode{2,Box::EXPAND|Box::FILL}),false)
				{
				m_pal_view.callback(*this,0);
				}

			void indexSelected(PaletteView& palview,int id)
				{
				}

		private:
			Box m_box;
				PaletteView m_pal_view;
				Box m_hue;
					ColorView m_hue_min;
					Slider m_hue_slider;
				Box m_sat;
					Slider m_sat_slider;
				Box m_lightness;
					Slider m_lightness_slider;			
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
