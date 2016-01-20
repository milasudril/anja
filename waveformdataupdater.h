#ifdef __WAND__
target[name[waveformdataupdater.h] type[include]]
dependency[waveformdataupdater.o]
#endif

#ifndef WAVEFORMDATAUPDATER_H
#define WAVEFORMDATAUPDATER_H

#include "waveformdataview.h"

class SessionView;

class WaveformDataUpdater:public WaveformDataView::EventHandler
	{
	public:
		WaveformDataUpdater():r_view(nullptr)
			{}

		void onSourceChange(WaveformDataView& source
			,const char* filename_new);

		void onDescriptionChange(WaveformDataView& source
			,const char* description_new);

		void onColorChange(WaveformDataView& source,const ColorRGBA& color_new);

		void onColorChange(WaveformDataView& source,const char* colorstr);

		void onGainChange(WaveformDataView& source,float value);

		void onGainRandomChange(WaveformDataView& source,float value);

		void onOptionSet(WaveformDataView& source,uint32_t option);

		void onOptionUnset(WaveformDataView& source,uint32_t option);

		void onChannelChange(WaveformDataView& source,uint32_t channel);

		void sessionViewSet(SessionView* view)
			{r_view=view;}

	private:
		SessionView* r_view;
	};

#endif
