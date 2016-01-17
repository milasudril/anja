#ifdef __WAND__
target[name[mixer.h] type[include]]
dependency[mixer.o]
#endif

#ifndef MIXER_H
#define MIXER_H

#include "framework/widget.h"
#include "framework/array_simple.h"

class GuiContainer;
class BoxHorizontal;
class ChannelStrip;
class ChannelData;

class Mixer:public Widget
	{
	public:
		static Mixer* create(GuiContainer& parent,ChannelData* channels
			,unsigned int n_channels);

		void destroy();

		const GuiHandle& handleNativeGet() const;

	private:
		Mixer(GuiContainer& parent,ChannelData* channels
			,unsigned int n_channels);
		~Mixer();
		BoxHorizontal* m_box;
		ArraySimple<ChannelStrip*> m_strips;
	};

#endif

