#ifdef __WAND__
target[name[waveformdataupdater.o] type[object]]
#endif

#include "waveformdataupdater.h"
#include "waveformdata.h"
#include "sessionview.h"
#include "colorstring.h"

void WaveformDataUpdater::onSourceChange(WaveformDataView& source
	,const char* filename_new)
	{
	auto& wd=source.waveformDataGet();
	try
		{
		wd.fileLoad(filename_new);
		}
	catch(const char* message)
		{
	//	messageDisplay(*_context,message,"Anja",MESSAGE_TYPE_ERROR);
		source.notifyUser(message,"Anja wave file loader",MESSAGE_TYPE_ERROR);
		}
	source.update();
	}

void WaveformDataUpdater::onDescriptionChange(WaveformDataView& source
	,const char* description_new)
	{
	auto& wd=source.waveformDataGet();
	wd.descriptionSet(description_new);
	r_view->keyboardViewUpdate();
	}

void WaveformDataUpdater::onColorChange(WaveformDataView& source
	,const ColorRGBA& color_new)
	{
	auto& wd=source.waveformDataGet();
	wd.keyColorSet(color_new);
	r_view->keyboardViewUpdate();
	}

void WaveformDataUpdater::onColorChange(WaveformDataView& source
	,const char* colorstr)
	{
	auto& wd=source.waveformDataGet();
	wd.keyColorSet(colorFromString(colorstr));
	r_view->keyboardViewUpdate();
	}

void WaveformDataUpdater::onGainChange(WaveformDataView& source
	,float value)
	{
	auto& waveform=source.waveformDataGet().waveformGet();
	waveform.gainSet(value);
	}

void WaveformDataUpdater::onGainRandomChange(WaveformDataView& source
	,float value)
	{
	auto& waveform=source.waveformDataGet().waveformGet();
	waveform.gainRandomSet(value);
	}

void WaveformDataUpdater::onOptionSet(WaveformDataView& source,uint32_t option)
	{
	auto& waveform=source.waveformDataGet().waveformGet();
	waveform.flagSet(option);
	}

void WaveformDataUpdater::onOptionUnset(WaveformDataView& source,uint32_t option)
	{
	auto& waveform=source.waveformDataGet().waveformGet();
	waveform.flagUnset(option);
	}

void WaveformDataUpdater::onChannelChange(WaveformDataView& source,uint32_t channel)
	{
	auto& waveform=source.waveformDataGet().waveformGet();
	waveform.channelSet(channel);
	}

