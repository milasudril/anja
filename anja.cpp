//@	{"targets":[{"name":"anja","type":"application"}]}

#include "ui/uicontext.hpp"
#include "ui/window.hpp"
#include "ui/sessioneditor.hpp"
#include "sessiondata/session.hpp"
#include <cstdio>
#include <time.h>

namespace Anja
	{
	class SessionControl
		{
		public:
			SessionControl(UiContext& ctx):r_ctx(ctx)
				{}

			void closing(Window& win)
				{r_ctx.exit();}

			UiContext::RunStatus idle(UiContext& ctx)
				{
				timespec req{0,0};
				nanosleep(&req,NULL);
				return UiContext::RunStatus::CONTINUE;
				}

		private:
			UiContext& r_ctx;
		};
	}

int main(int argc, char **argv)
	{
	try
		{
		Anja::UiContext ctx;
		Anja::Session session;
		session.load("testbank/testbank.txt");
		Anja::Window mainwin(session.filenameGet().begin(),0);
		Anja::SessionEditorBase editor(mainwin,session);
		Anja::SessionControl ctrl(ctx);
		mainwin.callback(ctrl);
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
