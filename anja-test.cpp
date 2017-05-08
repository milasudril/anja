//@	{"targets":[{"name":"anja-test","type":"application"}]}

#include "ui/window.hpp"
#include "ui/uicontext.hpp"
#include "ui/colorpicker.hpp"
#include "ui/dialog.hpp"

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

int main(int argc, char **argv)
	{
	try
		{
		Anja::UiContext ctx;
		ctx.dark(1);
		SessionControl ctrl(ctx);
		Anja::Window mainwin("Test");
		mainwin.callback(ctrl,0);
		Anja::Dialog<Anja::ColorPicker> picker(mainwin,"Choose color");
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
