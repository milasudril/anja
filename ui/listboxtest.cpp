//@	{"targets":[{"name":"listboxtest","type":"application"}]}

#include "listbox.hpp"
#include "window.hpp"
#include "uicontext.hpp"

int main()
	{
	Anja::UiContext ctx;
	Anja::Window mainwin("Listbox");

	struct Ui
		{
		auto idle(Anja::UiContext& ctx)
			{return Anja::UiContext::RunStatus::WAIT;}

		typedef int MessageId;

		typedef int MessageParam;

		void process(Anja::UiContext& ctx,MessageId id,MessageParam param)
			{}

		} ui;

	Anja::Listbox listbox(mainwin);
	listbox.append("Foo")
		.append("Bar")
		.append("Kaka");
	mainwin.show();
	ctx.run(ui);

	return 0;
	}
