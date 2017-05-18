//@	{"targets":[{"name":"anja-test","type":"application"}]}

#include "ui/window.hpp"
#include "ui/uicontext.hpp"
#include "ui/message.hpp"
#include "common/error.hpp"

#include <cstdio>
#include <cstdint>

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
		Anja::Message msg(mainwin,"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed arcu mi, scelerisque in sapien in, finibus viverra lorem. Fusce et efficitur augue.",Anja::Message::Type::ERROR);
		mainwin.callback(ctrl,0);
		mainwin.show();
		ctx.run(ctrl);
		}
	catch(const Anja::Error& e)
		{
		fprintf(stderr,"Error: %s\n",e.message());
		return -1;
		}
	return 0;
	}
