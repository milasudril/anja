#ifdef __WAND__
target[name[channelview.o] type[object]]
#endif

#include "channelview.h"
#include "channelstrip.h"
#include "channeldata.h"
#include "framework/boxhorizontal.h"

ChannelView* ChannelView::create(GuiContainer& parent
	,ChannelStrip::EventHandler& handler,ChannelData* channels
	,unsigned int n_channels)
	{return new ChannelView(parent,handler,channels,n_channels);}

void ChannelView::destroy()
	{
	delete this;
	}

const GuiHandle& ChannelView::handleNativeGet() const
	{return m_box->handleNativeGet();}

ChannelView::ChannelView(GuiContainer& parent
	,ChannelStrip::EventHandler& handler,ChannelData* channels
	,unsigned int n_channels):
	m_strips(n_channels)
	{
	m_box=BoxHorizontal::create(parent);
	m_box->slaveAssign(*this);
	m_box->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
		|BoxHorizontal::INSERTMODE_FILL);

	for(unsigned int k=0;k<n_channels;++k)
		{
		m_strips[k]=ChannelStrip::create(*m_box,handler,k);
		m_strips[k]->channelDataSet(channels[k]);
		}
	}

ChannelView::~ChannelView()
	{
	auto n_channels=m_strips.length();
	for(unsigned int k=0;k<n_channels;++k)
		{
		m_strips[k]->destroy();
		}
	m_box->slaveRelease();
	}

void ChannelView::channelDataSet(const ChannelData& data,unsigned int channel)
	{
	m_strips[channel]->channelDataSet(data);
	}
