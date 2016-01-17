#ifdef __WAND__
target[name[mixer.h] type[include]]
dependency[mixer.o]
#endif

#ifndef MIXER_H
#define MIXER_H

#include "framework/widget.h"

class GuiContainer;
class BoxHorizontal;
class Slider;

class Mixer:public Widget
	{
	public:
		static Mixer* create(GuiContainer& parent);

		void destroy();

		const GuiHandle& handleNativeGet() const;

	private:
		Mixer(GuiContainer& parent);
		~Mixer();
		BoxHorizontal* m_box;

	//	TODO The number should be fetched from the backend
		Slider* m_sliders[16];
	};

#endif

