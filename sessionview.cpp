#ifdef __WAND__
target[name[sessionview.o] type[object]]
#endif

#include "sessionview.h"
#include "session.h"
#include "waveformdataview.h"
#include "audioconnection.h"
#include "keyboardcontroller.h"
#include "waveformdataupdater.h"
#include "channelview.h"
#include "sessioncontrol.h"

#include "framework/boxvertical.h"
#include "framework/boxhorizontal.h"
#include "framework/keyboardview.h"
#include "framework/tabview.h"
#include "framework/titleview.h"

SessionView* SessionView::create(GuiContainer& parent,Session& session
	,TitleView& title_view
	,KeyboardView::EventHandler& keyboard_input
	,WaveformDataView::EventHandler& data_eventhandler
	,WaveformRangeView::EventHandler& rangeview_handler
	,ChannelStrip::EventHandler& channelstrip_handler)
	{
	return new SessionView(parent,session,title_view,keyboard_input
		,data_eventhandler,rangeview_handler,channelstrip_handler);
	}

void SessionView::destroy()
	{delete this;}

const GuiHandle& SessionView::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

SessionView::SessionView(GuiContainer& parent,Session& session
	,TitleView& title_view
	,KeyboardView::EventHandler& keyboard_input
	,WaveformDataView::EventHandler& data_eventhandler
	,WaveformRangeView::EventHandler& rangeview_handler
	,ChannelStrip::EventHandler& channelstrip_handler):r_tw(title_view)
	,m_fullscreen_state(0)
	{
	m_box=BoxHorizontal::create(parent);
	m_box->slaveAssign(*this);
	m_control=SessionControl::create(*m_box,session,*this);

	m_box->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
		|BoxHorizontal::INSERTMODE_FILL);

	m_box_right=BoxVertical::create(*m_box);
	m_box_right->insertModeSet(BoxVertical::INSERTMODE_EXPAND
		|BoxVertical::INSERTMODE_FILL);
	m_keyboard=KeyboardView::create(*m_box_right,session.keyboardLayoutGet()
		,keyboard_input);
	m_box_right->insertModeSet(BoxVertical::INSERTMODE_END
		|BoxVertical::INSERTMODE_FILL);
	m_tabs=TabView::create(*m_box_right);

	m_dataview=WaveformDataView::create(*m_tabs,data_eventhandler,rangeview_handler);
	m_tabs->tabTitleSet(0,"Waveform data");

	m_mixer=ChannelView::create(*m_tabs,channelstrip_handler
		,session.channelDataBegin(),session.channelsCountGet());
	m_tabs->tabTitleSet(1,"Channel mixer");

	sessionSet(session);
	}

SessionView::~SessionView()
	{
	m_mixer->destroy();
	m_dataview->destroy();
	m_tabs->destroy();
	m_keyboard->destroy();
	m_box_right->destroy();
	m_control->destroy();
	m_box->slaveRelease();
	}

void SessionView::sessionSet(Session& session)
	{
	r_session=&session;
	m_keyboard->keyboardLayoutSet(session.keyboardLayoutGet());
	m_dataview->channelSelectorInit(session.channelDataBegin()
		,session.channelsCountGet());

	m_mixer->channelDataSet(session.channelDataBegin(),session.channelsCountGet());

	ArrayDynamicShort<char> title("Anja - ");
	title.truncate().append(session.titleGet());
	r_tw.titleSet( title.begin() );

	slotDisplay(session.slotActiveGet());
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
