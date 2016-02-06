#ifdef __WAND__
target[name[channelview.o] type[object]]
#endif

#include "channelview.h"
#include "channelstrip.h"
#include "channeldata.h"
#include "framework/boxhorizontal.h"
#include "framework/scrollwindow.h"
#include "framework/slider.h"

ChannelView* ChannelView::create(GuiContainer& parent
	,ChannelStrip::EventHandler& handler,const ChannelData* channels
	,unsigned int n_channels)
	{return new ChannelView(parent,handler,channels,n_channels);}

void ChannelView::destroy()
	{
	delete this;
	}

const GuiHandle& ChannelView::handleNativeGet() const
	{return m_scroll->handleNativeGet();}

ChannelView::ChannelView(GuiContainer& parent
	,ChannelStrip::EventHandler& handler,const ChannelData* channels
	,unsigned int n_channels):
	m_strips(n_channels)
	{
	m_box=BoxHorizontal::create(parent);
	m_box->slaveAssign(*this);


	m_box->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
		|BoxHorizontal::INSERTMODE_FILL);

	m_scroll=ScrollWindow::create(*m_box,ScrollWindow::MODE_HORIZONTAL);
	m_box_channels=BoxHorizontal::create(*m_scroll);
	m_box_channels->insertModeSet(0);

	for(unsigned int k=0;k<n_channels;++k)
		{
		m_strips[k]=ChannelStrip::create(*m_box_channels,handler,k);
		m_strips[k]->channelDataSet(channels[k]);
		}

	m_box->insertModeSet(0);
	m_master_gain=Slider::create(*m_box,"Master\ngain/dB",0);
	}

ChannelView::~ChannelView()
	{
	m_master_gain->destroy();
	auto n_channels=m_strips.length();
	for(unsigned int k=n_channels;k>0;--k)
		{m_strips[k-1]->destroy();}
	m_box_channels->destroy();
	m_scroll->destroy();
	m_box->slaveRelease();
	m_box->destroy();
	}

void ChannelView::channelDataSet(const ChannelData& data,unsigned int channel)
	{
	m_strips[channel]->channelDataSet(data);
	}

void ChannelView::channelDataSet(const ChannelData* channels,unsigned int n_channels)
	{
	for(unsigned int k=0;k<n_channels;++k)
		{
		m_strips[k]->channelDataSet(channels[k]);
		}
	}
