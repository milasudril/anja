#ifdef __WAND__
target[name[mixer.o] type[object]]
#endif

#include "mixer.h"
#include "framework/boxhorizontal.h"
#include "framework/slider.h"
#include <cstdio>

Mixer* Mixer::create(GuiContainer& parent)
	{return new Mixer(parent);}

void Mixer::destroy()
	{
	delete this;
	}

const GuiHandle& Mixer::handleNativeGet() const
	{return m_box->handleNativeGet();}

Mixer::Mixer(GuiContainer& parent)
	{
	m_box=BoxHorizontal::create(parent);
	m_box->slaveAssign(*this);
	m_box->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
		|BoxHorizontal::INSERTMODE_FILL);

	for(unsigned int k=0;k<16;++k)
		{
		char buffer[16];
		sprintf(buffer,"Ch %u",k+1);
		m_sliders[k]=Slider::create(*m_box,buffer,k,0);
		}
	}

Mixer::~Mixer()
	{
	for(unsigned int k=0;k<16;++k)
		{
		m_sliders[k]->destroy();
		}
	m_box->slaveRelease();
	}
