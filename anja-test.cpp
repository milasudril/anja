//@	{"targets":[{"name":"anja-test","type":"application"}]}

#include "ui/paletteview.hpp"
#include "ui/window.hpp"
#include "ui/uicontext.hpp"
#include "ui/box.hpp"
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
				{
				m_pal_view.callback(*this,0);
				}

			void indexSelected(PaletteView& palview,int id)
				{
				printf("Selected %d\n",palview.selection());
				palview.selection(13);
				}

		private:
			Box m_box;
				PaletteView m_pal_view;
			
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
