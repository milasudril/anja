//@	{"targets":[{"name":"anja-test","type":"application"}]}

#include "ui/window.hpp"
#include "ui/uicontext.hpp"
#include "ui/channelstrip.hpp"
#include "sessiondata/session.hpp"

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

namespace Anja
	{
	class MixerConsole
		{
		public:
			explicit MixerConsole(Container& cnt,Session& session):
				m_strip_box(cnt,false)
					,m_strips(session.channelsCountGet()
						,[this,&session](ChannelStrip* mem,int k)
							{
							new(mem)ChannelStrip(m_strip_box.insertMode({0,Box::EXPAND|Box::FILL}),session.channelViewGet(k));
							m_separators.append(Separator(m_strip_box.insertMode({2,Box::EXPAND|Box::FILL}),true));
							})
					,m_master(m_strip_box.insertMode({2,0}),true)
						,m_master_label(m_master,"Master out")
						,m_master_slider(m_master.insertMode({0,Box::EXPAND|Box::FILL}),true)
						,m_master_entry(m_master.insertMode({0,0}))
				{
				m_master_entry.small(1).width(7);
				}

		private:
			Box m_strip_box;
				ArraySimple<ChannelStrip> m_strips;
				ArrayDynamicShort<Separator> m_separators;
				Box m_master;
					Label m_master_label;
					Slider m_master_slider;
					TextEntry m_master_entry;
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
		Anja::Session session;
		session.load("testbank/testbank.txt");
		Anja::MixerConsole mixer(mainwin,session);
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
