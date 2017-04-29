//@	{
//@	 "targets":[{"name":"waveformview.o","type":"object"}]
//@	}

#include "waveformview.hpp"
#include "sessionfilerecord.hpp"
#include "../common/pathutils.hpp"
#include <time.h>

using namespace Anja;

namespace
	{
	class IdGenerator
		{
		public:
			IdGenerator() noexcept:m_value(time(NULL)){}

			uint32_t next() noexcept
				{
				++m_value;
				return m_value;
				}

		private:
			uint32_t m_value;
		};
	}

static IdGenerator s_id_gen;

static String filenameGet(const String& filename,const String& load_path)
	{
	if(absoluteIs(filename))
		{return filename;}
	auto fullpath=load_path;
	fullpath.append(filename);
	return fullpath;
	}

static String filenameGenerate(const String& label)
	{
	String ret(label);
	if(ret.length()!=0)
		{ret.append('-');}
	char buff[9];
	sprintf(buff,"%x",s_id_gen.next());
	ret.append(buff).append(".wav");
	return ret;
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
	if(r_waveform.flagsGet()&Waveform::RECORDED)
		{
		auto filename=::filenameGet(filenameGenerate(r_waveform_data.keyLabelGet()),dir_current);
		r_waveform_data.filenameSet(makeRelativeTo(filename.begin(),dir_current.begin()));
		r_waveform.fileSave(filename.begin());
		}
	
	auto filename_out=r_waveform_data.filenameGet();
	if(*filename_out.begin()!='\0')
		{filename_out=makeRelativeTo(filename_out.begin(),dir_current.begin());}
	rec.propertySet(String("Filename"),filename_out);
	r_waveform.dataGet(rec);
	r_waveform_data.dataGet(rec);	
	}
