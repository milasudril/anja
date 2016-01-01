#ifdef __WAND__
target[name[waveformrangetrimmer.o] type[object]]
#endif

#include "waveformrangetrimmer.h"
#include "waveform.h"
#include "meansquare.h"
#include "units.h"

void WaveformRangeTrimmer::beginUpdate(WaveformRangeView& view,uint32_t position)
	{
	view.waveformGet().offsetBeginSet(position);
	view.cursorsUpdate();
	}

void WaveformRangeTrimmer::endUpdate(WaveformRangeView& view,uint32_t position)
	{
	view.waveformGet().offsetEndSet(position);
	view.cursorsUpdate();
	}

void WaveformRangeTrimmer::beginAutotrim(WaveformRangeView& view,float threshold)
	{
	auto& range=view.waveformGet();
	auto length=range.lengthFull();
	ArraySimple<float> meansquare(length);
	waveformRenderPrepare(view,meansquare.begin(),length);

	auto ptr_begin=meansquare.begin();
	auto ptr_end=meansquare.end();
	while(ptr_begin!=ptr_end)
		{
		if(*ptr_begin>=threshold)
			{break;}
		++ptr_begin;
		}
	auto index=ptr_begin-meansquare.begin();
	range.offsetBeginSet(index);
	view.cursorsUpdate();
	}

void WaveformRangeTrimmer::endAutotrim(WaveformRangeView& view,float threshold)
	{
	auto& range=view.waveformGet();
	auto length=range.lengthFull();
	ArraySimple<float> meansquare(length);
	waveformRenderPrepare(view,meansquare.begin(),length);

	auto ptr_begin=meansquare.begin();
	auto ptr_end=meansquare.end();
	while(ptr_begin!=ptr_end)
		{
		--ptr_end;
		if(*ptr_end>=threshold)
			{break;}
		}
	auto index=ptr_end-meansquare.begin();
	range.offsetEndSet(index);
	view.cursorsUpdate();
	}

void WaveformRangeTrimmer::waveformRenderPrepare(WaveformRangeView& view
	,float* buffer_out,size_t length)
	{
	auto& range=view.waveformGet();
	MeanSquare(secondsToFrames(1e-3,range.sampleRateGet()))
		.compute(range.beginFull(),buffer_out,length);
	}
