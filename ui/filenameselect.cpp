//@	 {"targets":[{"name":"filenameselect.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "filenameselect.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Anja;

bool Anja::filenameSelect(const Container& cnt,const char* working_dir
	,std::string& filename_in,FilenameSelectMode mode,FilenameSelectFilter cb
	,void* cb_obj,const char* filter_name)
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
	if(filename_in.length()!=0)
		{
		auto filename_tot=std::string(working_dir);
		filename_tot+=filename_in;
		gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dlg),filename_tot.c_str());
		}
	else
		{gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dlg),working_dir);}
	gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(dlg),working_dir,NULL);

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
