//@	{
//@	 "targets":[{"name":"waveformproxy.o","type":"object"}]
//@	}

#include "waveformproxy.hpp"
#include "sessionfilerecord.hpp"
#include "../common/pathutils.hpp"
#include <time.h>

using namespace Anja;

static String filename_get(const char* filename,const String& load_path)
	{
	if(absoluteIs(filename))
		{return String(filename);}
	auto fullpath=load_path;
	fullpath.append(filename);
	return fullpath;
	}

WaveformProxy& WaveformProxy::load(const SessionFileRecord& rec,progress_callback cb,void* cb_obj)
	{
	r_waveform_data->load(rec);
	if(r_waveform_data->filenameGet().length()==0)
		{r_waveform->load(rec);}
	else
		{
		auto f=::filename_get(r_waveform_data->filenameGet().begin(),*r_dir_current);
		struct load_callback
			{
			decltype(cb) m_cb;
			decltype(cb_obj) m_cb_obj;
			WaveformProxy& r_proxy;
			void progressLoad(Waveform& wf,float status)
				{m_cb(m_cb_obj,r_proxy,status);}
			} load_cb{cb,cb_obj,*this};
		r_waveform_data->filename(f.begin());
		r_waveform->load(rec,f.begin(),load_cb);
		r_waveform_data->dirtyClear();
		}
	return *this;
	}

String WaveformProxy::filename() const
	{
	return makeRelativeTo(r_waveform_data->filenameGet().begin(),r_dir_current->begin());
	}

WaveformProxy& WaveformProxy::filename(const Anja::String& str)
	{
	auto dir=*r_dir_current;
	dir.append(str);
	r_waveform_data->filename(std::move(dir));
	return *this;
	}

const WaveformProxy& WaveformProxy::store(SessionFileRecord& rec) const
	{
	if(r_waveform->flags()&Waveform::RECORDED)
		{r_waveform->waveformSave(r_waveform_data->filenameGet().begin());}

	rec.propertySet(String("Filename"),filename());
	r_waveform->store(rec);
	r_waveform_data->store(rec);
	return *this;
	}


WaveformProxy& WaveformProxy::waveformLoad(const char* filename,progress_callback cb,void* cb_obj)
	{
	struct load_callback
		{
		decltype(cb) m_cb;
		decltype(cb_obj) m_cb_obj;
		WaveformProxy& r_proxy;
		void progressLoad(Waveform& wf,float status)
			{m_cb(m_cb_obj,r_proxy,status);}
		} load_cb{cb,cb_obj,*this};

	if(absoluteIs(filename))
		{
		r_waveform->waveformLoad(filename,load_cb);
		r_waveform_data->filename(filename);
		}
	else
		{
		auto fullpath=::filename_get(filename,*r_dir_current);
		r_waveform->waveformLoad(fullpath.begin(),load_cb);
		r_waveform_data->filename(std::move(fullpath));
		}
	return *this;
	}

const WaveformProxy& WaveformProxy::waveformSave(const char* filename) const
	{
	if(absoluteIs(filename))
		{
		r_waveform->waveformSave(filename);
		r_waveform_data->filename(makeRelativeTo(filename,r_dir_current->begin()).begin());
		}
	else
		{
		auto fullpath=::filename_get(filename,*r_dir_current);
		r_waveform->waveformSave(fullpath.begin());
		r_waveform_data->filename(std::move(fullpath));
		}
	return *this;
	}

bool WaveformProxy::loadPossible(const char* filename) const
	{
	if(absoluteIs(filename))
		{return r_waveform->loadPossible(filename);}

	return r_waveform->loadPossible(filename);
	}

bool WaveformProxy::waveformLoaded(const char* filename) const
	{
	if(absoluteIs(filename))
		{return r_waveform_data->filenameGet()==filename;}
	auto cmp=*r_dir_current;
	cmp.append(filename);
	return realpath(cmp.begin())==r_waveform_data->filenameGet();
	}
