//@	 {"targets":[{"name":"filenameselect.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "filenameselect.hpp"
#include "container.hpp"
#include "../common/thread.hpp"
#include "../common/pipe.hpp"
#include <gtk/gtk.h>

using namespace Anja;

bool Anja::filenameSelect(const Container& cnt,std::string& filename_in
	,FilenameSelectMode mode,FilenameSelectFilter cb,void* cb_obj
	,const char* filter_name)
	{
	auto action=mode==FilenameSelectMode::OPEN?
		 GTK_FILE_CHOOSER_ACTION_OPEN : GTK_FILE_CHOOSER_ACTION_SAVE;

	const char* text_ok=mode==FilenameSelectMode::OPEN? "Open" : "Save";
	const char* text_title=mode==FilenameSelectMode::OPEN? 
		"Select file to open" : "Choose filename";

	auto parent=cnt.toplevel();
	auto dlg=gtk_file_chooser_dialog_new(text_title,GTK_WINDOW(parent)
		,action,"Cancel",GTK_RESPONSE_CANCEL,text_ok,GTK_RESPONSE_ACCEPT,NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dlg)
		,mode==FilenameSelectMode::SAVE);
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dlg),filename_in.c_str());
	auto filter=gtk_file_filter_new();
	gtk_file_filter_set_name(filter,filter_name);
	std::pair<FilenameSelectFilter,void*> cb_obj_gtk{cb,cb_obj};
	auto cb_gtk=[](const GtkFileFilterInfo* filter_info,gpointer data)
		{
		auto obj=reinterpret_cast<std::pair<FilenameSelectFilter,void*>*>(data);
		if(obj->first(obj->second,filter_info->filename))
			{return 1;}
		return 0;
		};

	gtk_file_filter_add_custom(filter,GTK_FILE_FILTER_FILENAME,cb_gtk,&cb_obj_gtk,nullptr);
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dlg),filter);

	if(gtk_dialog_run(GTK_DIALOG(dlg))==GTK_RESPONSE_ACCEPT)
		{
		auto filename_out=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));
		try
			{filename_in=filename_out;}
		catch(...)
			{
			gtk_widget_destroy(dlg);
			g_free(filename_out);
			throw;
			}
		gtk_widget_destroy(dlg);
		g_free(filename_out);
		return 1;
		}
	gtk_widget_destroy(dlg);
	return 0;
	}

namespace
	{
	class FilenameSelectThreadEntry
		{
		public:
			explicit FilenameSelectThreadEntry(const char* filename_in
				,FilenameSelectMode mode,const char* context_name,Container& cnt
				,FilenameSelectCallback cb,void* cb_obj):
				 m_filename_in(filename_in),m_mode(mode),m_context_name(context_name)
				,r_container(cnt),m_cb(cb),r_cb_obj(cb_obj)
				{}

			void operator()()
				{
				m_state_current=State::STATUS;
			//	Spawn second process and read data back
				const char* const cmd[]=
					{
					 "anja","anja","filename_select"
					,m_filename_in.c_str()
					,m_mode==FilenameSelectMode::OPEN?"open":"save"
					,m_context_name.c_str()
					,nullptr
					};
			//	Read results from the pipe. Data will be processed write below.
				pipe(cmd,*this); 
			//	Notify main thread. Main thread synchronizes with A
			//	activates cnt, and calls cb.
				g_idle_add([](void* obj)
					{
					reinterpret_cast<FilenameSelectThreadEntry*>(obj)->mainNotify();
					return G_SOURCE_REMOVE;
					},this);
				}

			void write(const int8_t* buffer,size_t N)
				{
				while(N!=0)
					{
					switch(m_state_current)
						{
						case State::STATUS:
							switch(*buffer)
								{
								case ':':
									if(m_ret_buff=="cancel")
										{
										m_status=m_ret_buff;
										m_state_current=State::DONE;
										}
									else
										{
										m_status=m_ret_buff;
										m_filename_in.clear();
										m_state_current=State::VALUE;
										}
									break;
								default:
									m_ret_buff+=*buffer;
								}
							break;
						case State::VALUE:
							m_filename_in+=*buffer;
							break;
						case State::DONE:
							break;
						}
					++buffer;
					--N;
					}
				}

			void mainNotify()
				{
				r_container.sensitive(true);
				m_cb(r_cb_obj,m_context_name.c_str(),m_filename_in.c_str());				
				}

		private:
			std::string m_filename_in;
			FilenameSelectMode m_mode;
			std::string m_context_name;
			Container& r_container;
			FilenameSelectCallback m_cb;
			void* r_cb_obj;
			std::string m_ret_buff;
			enum class State:int{STATUS,VALUE,DONE};
			State m_state_current;
			std::string m_status;
		};
	}

void Anja::filenameSelectRemote(Container& cnt,const char* filename_in
	,FilenameSelectMode mode,const char* context_name,FilenameSelectCallback cb
	,void* cb_obj)
	{
	cnt.sensitive(false);
	Thread<FilenameSelectThreadEntry> a(FilenameSelectThreadEntry
		{filename_in,mode,context_name,cnt,cb,cb_obj});
	}
