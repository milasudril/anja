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
#include "framework/boxvertical.h"
#include "framework/keyboardview.h"
#include "framework/tabview.h"

SessionView* SessionView::create(GuiContainer& parent,Session& session
	,KeyboardView::EventHandler& keyboard_input
	,WaveformDataView::EventHandler& data_eventhandler
	,WaveformRangeView::EventHandler& rangeview_handler
	,ChannelStrip::EventHandler& channelstrip_handler)
	{
	return new SessionView(parent,session,keyboard_input,data_eventhandler
		,rangeview_handler,channelstrip_handler);
	}

void SessionView::destroy()
	{delete this;}

const GuiHandle& SessionView::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

SessionView::SessionView(GuiContainer& parent,Session& session
	,KeyboardView::EventHandler& keyboard_input
	,WaveformDataView::EventHandler& data_eventhandler
	,WaveformRangeView::EventHandler& rangeview_handler
	,ChannelStrip::EventHandler& channelstrip_handler)
	{
	m_box=BoxVertical::create(parent);
	m_box->slaveAssign(*this);
	m_box->insertModeSet(BoxVertical::INSERTMODE_EXPAND
		|BoxVertical::INSERTMODE_FILL);
	m_keyboard=KeyboardView::create(*m_box,session.keyboardLayoutGet(),keyboard_input);
	m_box->insertModeSet(BoxVertical::INSERTMODE_END
		|BoxVertical::INSERTMODE_FILL);
	m_tabs=TabView::create(*m_box);

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
	m_box->slaveRelease();
	}

void SessionView::sessionSet(Session& session)
	{
	r_session=&session;
	m_keyboard->keyboardLayoutSet(session.keyboardLayoutGet());
	m_dataview->channelSelectorInit(session.channelDataBegin()
		,session.channelsCountGet());
	slotDisplay(session.slotActiveGet());
	}

void SessionView::slotDisplay(uint8_t slot)
	{
	m_dataview->waveformDataSet(r_session->waveformDataGet(slot));
	m_tabs->tabActiveSet(0);
	}

void SessionView::channelDisplay(uint8_t channel)
	{
	m_tabs->tabActiveSet(1);
	}

void SessionView::keyboardViewUpdate()
	{
	m_keyboard->update();
	}

void SessionView::channelNameUpdate(unsigned int id)
	{
	m_dataview->channelNameUpdate(r_session->channelDataGet(id),id);
	}
