//@	{"targets":[{"name":"listboxtest","type":"application"}]}

#include "window.hpp"
#include "uicontext.hpp"
#include "buttonlist.hpp"

int main()
	{
	Anja::UiContext ctx;
	Anja::Window mainwin("Listbox");

	struct Ui
		{
		Ui(Anja::UiContext& ctx):r_ctx(ctx){}
		auto idle(Anja::UiContext& ctx)
			{return Anja::UiContext::RunStatus::WAIT;}

		typedef int MessageId;

		typedef int MessageParam;

		void process(Anja::UiContext& ctx,MessageId id,MessageParam param)
			{}

		void closing(Anja::Window& win,int id)
			{r_ctx.exit();}

		void keyDown(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id){}
		void keyUp(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id){}
		void focusIn(Anja::Window& win,int id){}
		void focusOut(Anja::Window& win,int id){}

		Anja::UiContext& r_ctx;

		} ui(ctx);

	Anja::ButtonList listbox(mainwin,true);
	listbox.append("Foo")
		.append("Bar")
		.append("Kaka");
	mainwin.callback(ui,0);
	mainwin.show();
	ctx.run(ui);

	return 0;
	}
