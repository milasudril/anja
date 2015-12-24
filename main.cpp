#ifdef __WAND__
target[name[anja] type[application]]
#endif

#include "waveformrangeview.h"
#include "waveformdataview.h"
#include "waveformrange.h"
#include "meansquare.h"
#include "units.h"

#include "framework/window.h"
#include "framework/boxvertical.h"
#include "framework/keyboardlayout.h"
#include "framework/keyboardview.h"

#include "framework/array_simple.h"

#include <cmath>

class RangeAction:public WaveformRangeView::EventHandler
	{
	public:
		RangeAction()
			{}

		void beginUpdate(WaveformRangeView& view,uint32_t position)
			{
			view.waveformRangeGet().offsetBeginSet(position);
			view.cursorsUpdate();
			}

		void endUpdate(WaveformRangeView& view,uint32_t position)
			{
			view.waveformRangeGet().offsetEndSet(position);
			view.cursorsUpdate();
			}

		void beginAutotrim(WaveformRangeView& view,float threshold)
			{
			auto& range=view.waveformRangeGet();
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

		void endAutotrim(WaveformRangeView& view,float threshold)
			{
			auto& range=view.waveformRangeGet();
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

		void waveformRenderPrepare(WaveformRangeView& view
			,float* buffer_out,size_t length)
			{
			auto& range=view.waveformRangeGet();
			MeanSquare(secondsToFrames(1e-3,range.sampleRateGet()))
				.compute(range.beginFull(),buffer_out,length);
			}
	private:
	};

int main()
	{
	double f=1000;
	double T=1/f;
	size_t N_periods=6;
	double fs=48000;
	size_t N=N_periods*T*fs;
	ArraySimple<float> test_data(N);
	for(size_t k=0;k<N;++k)
		{
		auto t=k/fs;
		if(t>1e-3)
			{
			t-=1e-3;
			test_data[k]=cos(2*acos(-1)*f*t)*exp(-4*t*f);
			}
		else
			{test_data[k]=0;}
		}

	WaveformRange range{test_data.begin(),uint32_t(N),float(fs)};
	KeyboardLayout layout;

	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	mainwin->titleSet("Anja - New session");

	auto box=BoxVertical::create(*mainwin);
	box->insertModeSet(BoxVertical::INSERTMODE_EXPAND
		|BoxVertical::INSERTMODE_FILL);
	auto keyview=KeyboardView::create(*box,layout);

	RangeAction actions;
	box->insertModeSet(BoxVertical::INSERTMODE_END);
	auto waveformdata=WaveformDataView::create(*box,actions);
//	waveformview->waveformRangeSet(range);

	return 0;
	}
