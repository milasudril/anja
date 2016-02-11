#ifdef __WAND__
target[name[sessionview.o] type[object]]
#endif

#include "sessionview.h"
#include "session.h"
#include "audioconnection.h"
#include "channelview.h"

#include "framework/boxvertical.h"
#include "framework/boxhorizontal.h"
#include "framework/keyboardview.h"
#include "framework/tabview.h"
#include "framework/delimiter.h"
#include "framework/label.h"

SessionView* SessionView::create(GuiContainer& parent,Session& session
	,SessionActions::EventHandler& session_actions
	,KeyboardView::EventHandler& keyboard_input
	,WaveformDataView::EventHandler& data_eventhandler
	,WaveformRangeView::EventHandler& rangeview_handler
	,ChannelStripHandler::EventHandler& channelstrip_handler
	,SessionDataView::EventHandler& sessiondata_handler)
	{
	return new SessionView(parent,session,session_actions,keyboard_input
		,data_eventhandler,rangeview_handler,channelstrip_handler
		,sessiondata_handler);
	}

void SessionView::destroy()
	{delete this;}

const GuiHandle& SessionView::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

SessionView::SessionView(GuiContainer& parent,Session& session
	,SessionActions::EventHandler& session_actions
	,KeyboardView::EventHandler& keyboard_input
	,WaveformDataView::EventHandler& waveformdata_handler
	,WaveformRangeView::EventHandler& rangeview_handler
	,ChannelStripHandler::EventHandler& channelstrip_handler
	,SessionDataView::EventHandler& sessiondata_handler):r_parent(parent)
	,m_fullscreen_state(0)
	{
	m_box=BoxHorizontal::create(parent);
	m_box->slaveAssign(*this);
	m_actions=SessionActions::create(*m_box,session_actions,session);

	m_delimiter=Delimiter::create(*m_box);

	m_box->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
		|BoxVertical::INSERTMODE_FILL);

	m_vbox=BoxVertical::create(*m_box);

	m_vbox->insertModeSet(BoxVertical::INSERTMODE_LEFT);
	m_status=Label::create(*m_vbox,"Anja is offline. Click \"Start engine\" to activate audio output.");

	m_vbox->insertModeSet(0);
	m_delimiter_h=Delimiter::create(*m_vbox);

	m_vbox->insertModeSet(BoxVertical::INSERTMODE_EXPAND
		|BoxVertical::INSERTMODE_FILL);

	m_keyboard=KeyboardView::create(*m_vbox,session.keyboardLayoutGet()
		,keyboard_input);
	m_vbox->insertModeSet(BoxVertical::INSERTMODE_FILL|BoxVertical::INSERTMODE_EXPAND);
	m_tabs=TabView::create(*m_vbox);

	m_dataview=WaveformDataView::create(*m_tabs,waveformdata_handler,rangeview_handler
		,session.colorPresetsBegin(),session.colorPresetsCountGet());
	m_tabs->tabTitleSet(0,"Waveform data");

	m_mixer=ChannelView::create(*m_tabs,channelstrip_handler
		,session.channelDataBegin(),session.channelsCountGet()
		,session.colorPresetsBegin(),session.colorPresetsCountGet());
	m_tabs->tabTitleSet(1,"Channel mixer");

	m_sessiondata=SessionDataView::create(*m_tabs,session,sessiondata_handler);
	m_tabs->tabTitleSet(2,"Session properties");

	sessionSet(session);
	}

SessionView::~SessionView()
	{
	m_sessiondata->destroy();
	m_mixer->destroy();
	m_dataview->destroy();
	m_tabs->destroy();
	m_keyboard->destroy();
	m_delimiter_h->destroy();
	m_status->destroy();
	m_vbox->destroy();
	m_delimiter->destroy();
	m_actions->destroy();
	m_box->slaveRelease();
	m_box->destroy();
	}

void SessionView::sessionSet(Session& session)
	{
	r_session=&session;
	m_keyboard->keyboardLayoutSet(session.keyboardLayoutGet());
	m_dataview->channelSelectorInit(session.channelDataBegin()
		,session.channelsCountGet());

	m_mixer->channelDataSet(session.channelDataBegin(),session.channelsCountGet());
	m_mixer->masterGainSet(session.masterGainGet());
	m_sessiondata->sessionSet(session);

	ArrayDynamicShort<char> title("Anja - ");
	title.truncate().append(session.titleGet());
	r_parent.titleSet( title.begin() );

	slotDisplay(session.slotActiveGet());
	statusUpdate();
	}

void SessionView::slotDisplay(uint8_t slot)
	{
	m_dataview->waveformDataSet(r_session->waveformDataGet(slot));
	m_tabs->tabActiveSet(0);
	}

void SessionView::channelDisplay(unsigned int channel)
	{
	m_tabs->tabActiveSet(1);
	}

void SessionView::keyboardViewUpdate()
	{
	m_keyboard->update();
	}

void SessionView::channelNameUpdate(unsigned int channel)
	{
	m_dataview->channelNameUpdate(r_session->channelDataGet(channel),channel);
	}

void SessionView::channelColorUpdate(unsigned int channel)
	{
	m_mixer->channelDataSet(r_session->channelDataGet(channel),channel);
	}

void SessionView::fullscreenToggle()
	{
	m_fullscreen_state=!m_fullscreen_state;
	m_box->fullscreenSet(m_fullscreen_state);
	}

void SessionView::sessionTitleUpdate()
	{
	r_parent.titleSet(r_session->titleGet().begin());
	}

void SessionView::statusUpdate()
	{
	if(r_session->connectedIs())
		{
		if(r_session->restartNeeded())
			{m_status->titleSet("Changes requires the engine to be restarted.");}
		else
			{
			m_status->titleSet("Anja is now online. Use a JACK patchbay tool "
				"such as Catia or Carla to connect Anja to other parts of the system.");
			}
		}
	else
		{
		m_status->titleSet("Anja is currently offline. Click \"Start engine\" to activate audio output.");
		}
	}

