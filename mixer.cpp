#ifdef __WAND__
target[name[mixer.o] type[object]]
#endif

#include "mixer.h"
#include "channelstrip.h"
#include "channeldata.h"
#include "framework/boxhorizontal.h"

Mixer* Mixer::create(GuiContainer& parent,ChannelData* channels
	,unsigned int n_channels)
	{return new Mixer(parent,channels,n_channels);}

void Mixer::destroy()
	{
	delete this;
	}

const GuiHandle& Mixer::handleNativeGet() const
	{return m_box->handleNativeGet();}

Mixer::Mixer(GuiContainer& parent,ChannelData* channels,unsigned int n_channels):
	m_strips(n_channels)
	{
	m_box=BoxHorizontal::create(parent);
	m_box->slaveAssign(*this);
	m_box->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
		|BoxHorizontal::INSERTMODE_FILL);

	for(unsigned int k=0;k<n_channels;++k)
		{
		m_strips[k]=ChannelStrip::create(*m_box);
		m_strips[k]->channelDataSet(channels[k]);
		}
	}

Mixer::~Mixer()
	{
	auto n_channels=m_strips.length();
	for(unsigned int k=0;k<n_channels;++k)
		{
		m_strips[k]->destroy();
		}
	m_box->slaveRelease();
	}
