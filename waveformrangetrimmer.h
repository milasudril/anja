#ifdef __WAND__
target[name[waveformrangetrimmer.h] type[rangetrimmer.h]]
dependency[waveformrangetrimmer.o]
#endif

#include "waveformrangeview.h"

#ifndef WAVEFORMRANGETRIMMER_H
#define WAVEFORMRANGETRIMMER_H

class WaveformRangeTrimmer:public WaveformRangeView::EventHandler
	{
	public:
		void beginUpdate(WaveformRangeView& view,uint32_t position);

		void endUpdate(WaveformRangeView& view,uint32_t position);

		void beginAutotrim(WaveformRangeView& view,float threshold);

		void endAutotrim(WaveformRangeView& view,float threshold);

		void waveformRenderPrepare(WaveformRangeView& view
			,float* buffer_out,size_t length);
	};

#endif
