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

static String filename_get(const char* filename,const String& load_path)
	{
	if(absoluteIs(filename))
		{return String(filename);}
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

void WaveformView::load(const SessionFileRecord& rec)
	{
	r_waveform_data->dataSet(rec);
	if(r_waveform_data->filenameGet().length()==0)
		{r_waveform->dataSet(rec);}
	else
		{
		auto f=::filename_get(r_waveform_data->filenameGet().begin(),*r_dir_current);
		r_waveform->dataSet(rec,f.begin());
		r_waveform_data->filenameSet(f.begin());
		r_waveform_data->dirtyClear();
		}
	}

static String filename_generate(const String& str,const String& in_dir)
	{
	if(absoluteIs(str))
		{return makeRelativeTo(str.begin(),in_dir.begin());}
	return String(in_dir).append(str);
	}

String WaveformView::filenameGet() const
	{
	return makeRelativeTo(r_waveform_data->filenameGet().begin(),r_dir_current->begin());
	}

void WaveformView::store(SessionFileRecord& rec)
	{
	if(r_waveform->flagsGet()&Waveform::RECORDED)
		{r_waveform->fileSave(r_waveform_data->filenameGet().begin());}

	rec.propertySet(String("Filename"),filenameGet());
	r_waveform->dataGet(rec);
	r_waveform_data->dataGet(rec);
	}


void WaveformView::fileLoad(const char* filename)
	{
	if(absoluteIs(filename))
		{
		r_waveform->fileLoad(filename);
		r_waveform_data->filenameSet(filename);
		}
	else
		{
		auto fullpath=::filename_get(filename,*r_dir_current);
		r_waveform->fileLoad(fullpath.begin());
		r_waveform_data->filenameSet(std::move(fullpath));
		}
	}

void WaveformView::fileSave(const char* filename) const
	{
	if(absoluteIs(filename))
		{
		r_waveform->fileSave(filename);
		r_waveform_data->filenameSet(makeRelativeTo(filename,r_dir_current->begin()).begin());
		}
	else
		{
		auto fullpath=::filename_get(filename,*r_dir_current);
		r_waveform->fileSave(fullpath.begin());
		r_waveform_data->filenameSet(std::move(fullpath));
		}
	}

bool WaveformView::loadPossible(const char* filename) const
	{
	if(absoluteIs(filename))
		{return r_waveform->loadPossible(filename);}

	return r_waveform->loadPossible(filename);
	}

bool WaveformView::fileLoaded(const char* filename) const
	{
	if(absoluteIs(filename))
		{return r_waveform_data->filenameGet()==filename;}
	auto cmp=*r_dir_current;
	cmp.append(filename);
	return cmp==r_waveform_data->filenameGet();
	}
