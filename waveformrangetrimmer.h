#ifdef __WAND__
target[name[waveformrangetrimmer.h] type[rangetrimmer.h]]
dependency[waveformrangetrimmer.o]
#endif

#include "waveformrangeview.h"

#ifndef WAVEFORMRANGETRIMMER_H
#define WAVEFORMRANGETRIMMER_H

class WaveformData;

class WaveformRangeTrimmer:public WaveformRangeView::EventHandler
	{
	public:
		WaveformRangeTrimmer():r_wd(nullptr){}

		void beginUpdate(WaveformRangeView& view,uint32_t position);

		void endUpdate(WaveformRangeView& view,uint32_t position);

		void reverse(WaveformRangeView& view);

		void beginAutotrim(WaveformRangeView& view,float threshold);

		void endAutotrim(WaveformRangeView& view,float threshold);

		void waveformRenderPrepare(WaveformRangeView& view
			,float* buffer_out,size_t length);

		void waveformDataSet(WaveformData& wd)
			{r_wd=&wd;}

	private:
		WaveformData* r_wd;
	};

#endif
