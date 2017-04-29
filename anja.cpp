//@	{"targets":[{"name":"anja","type":"application"}]}

#include "ui/uicontext.hpp"
#include "ui/window.hpp"
#include "ui/tabview.hpp"
#include "ui/textentry.hpp"
#include "ui/paned.hpp"
#include <cstdio>

namespace Anja
	{
	class SessionControl
		{
		public:
			SessionControl(UiContext& ctx):r_ctx(ctx)
				{}

			void closing(Window& win)
				{r_ctx.exit();}
		private:
			UiContext& r_ctx;
		};
	}

int main(int argc, char **argv)
	{
	try
		{
		Anja::UiContext ctx;
		Anja::SessionControl ctrl(ctx);
		Anja::Window mainwin("New session",0);
		Anja::Paned paned(mainwin,true);
		Anja::TextEntry e0(paned
			.insertMode(Anja::Paned::InsertMode
				{Anja::Paned::SHRINK_ALLOWED|Anja::Paned::RESIZE|Anja::Paned::SCROLL_ALLOWED}),0);
		e0.small(1);
		Anja::TabView tabs(paned);
		tabs.tabTitle("Waveform data");
		Anja::TextEntry test(tabs,1);

		mainwin.callback(ctrl);
		mainwin.show();
		ctx.run();
		}
	catch(const char* err)
		{
		fprintf(stderr,"Error: %s\n",err);
		return -1;
		}
	return 0;
	}
