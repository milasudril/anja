//@	{
//@	 "targets":[{"name":"waveformview.o","type":"object"}]
//@	}

#include "waveformview.hpp"
#include "sessionfilerecord.hpp"
#include "../common/pathutils.hpp"

using namespace Anja;


static String filenameGet(const String& filename,const String& load_path)
	{
	if(absoluteIs(filename))
		{return filename;}
	auto fullpath=load_path;
	fullpath.truncate().append(filename).append('\0');
	return fullpath;
	}

void WaveformView::load(const SessionFileRecord& rec,const String& dir_current)
	{
	r_waveform.dataSet(rec);
	r_waveform_data.dataSet(rec);

	r_key.labelSet(r_waveform_data.keyLabelGet().begin());
	r_key.colorBackgroundSet(r_waveform_data.keyColorGet());

	if(r_waveform_data.filenameGet().length()!=0)
		{
		auto f=::filenameGet(r_waveform_data.filenameGet(),dir_current);
		r_waveform_data.filenameSet(f);
		r_waveform.fileLoad(f.begin());
		}
	}

void WaveformView::store(SessionFileRecord& rec,const String& dir_current)
	{
	}
