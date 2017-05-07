//@	{"targets":[{"name":"anja","type":"application"}]}

#include "ui/uicontext.hpp"
#include "ui/window.hpp"
#include "ui/sessioneditor.hpp"
#include "ui/paned.hpp"
#include "ui/label.hpp"
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

			void closing(Window& win,int id)
				{r_ctx.exit();}

			UiContext::RunStatus idle(UiContext& ctx)
				{
				return UiContext::RunStatus::WAIT;
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
		ctx.dark(1);
		Anja::Session session;
		session.load("testbank/testbank.txt");
		Anja::Window mainwin(session.filenameGet().begin());
		Anja::Paned paned(mainwin,true);
		Anja::Label lbl(paned,"Keyboard view placeholder");
		Anja::SessionEditorBase editor(paned.insertMode({Anja::Paned::SHRINK_ALLOWED|Anja::Paned::RESIZE}),session);
		Anja::SessionControl ctrl(ctx);
		mainwin.callback(ctrl,0);
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
