#ifdef __WAND__
target[
	name[filenamepicker.o]
	type[object]
	platform[;GNU/Linux]
	dependency[gtk-3;external]
	dependency[gdk-3;external]
	dependency[pangocairo-1.0;external]
	dependency[pango-1.0;external]
	dependency[atk-1.0;external]
	dependency[cairo-gobject;external]
	dependency[cairo;external]
	dependency[gdk_pixbuf-2.0;external]
	dependency[gio-2.0;external]
	dependency[gobject-2.0;external]
	dependency[glib-2.0;external]
	]
#endif

#include "filenamepicker.h"
#include "widget.h"
#include "guihandle.h"
#include <gtk/gtk.h>

class FilenamePickerGtk:public FilenamePicker
	{
	public:
		FilenamePickerGtk(const Widget&owner
			,const char* filename_in,uint32_t mode);

		~FilenamePickerGtk();

		const char* filenameGet() const
			{return m_filename_out;}

	private:
		char* m_filename_out;
		GtkWidget* m_dialog;
	};

FilenamePickerGtk::FilenamePickerGtk(
	 const Widget& owner
	,const char* filename_in,uint32_t mode):
	m_filename_out(nullptr)
	{
	auto parent=gtk_widget_get_toplevel(owner.handleNativeGet());
	auto action=mode==MODE_OPEN?
		 GTK_FILE_CHOOSER_ACTION_OPEN: GTK_FILE_CHOOSER_ACTION_SAVE;

	const char* text_ok=mode==MODE_OPEN? "Open" : "Save";
	const char* text_title=mode==MODE_OPEN? "Select file to open" : "Choose filename";

	m_dialog=gtk_file_chooser_dialog_new(text_title,(GtkWindow*)parent
		,action,"Cancel",GTK_RESPONSE_CANCEL,text_ok,GTK_RESPONSE_ACCEPT,NULL);

	gtk_file_chooser_set_filename((GtkFileChooser*)m_dialog,filename_in);
	if(gtk_dialog_run(GTK_DIALOG(m_dialog))==GTK_RESPONSE_ACCEPT)
		{m_filename_out=gtk_file_chooser_get_filename((GtkFileChooser*)m_dialog);}
	}

FilenamePickerGtk::~FilenamePickerGtk()
	{
	if(m_filename_out!=nullptr)
		{g_free(m_filename_out);}
	gtk_widget_destroy(m_dialog);
	}

FilenamePicker* FilenamePicker::instanceCreate(const Widget& owner
	,const char* filename_in
	,uint32_t mode)
	{return new FilenamePickerGtk(owner,filename_in,mode);}

void FilenamePicker::destroy(FilenamePicker* picker)
	{
	delete (FilenamePickerGtk*)picker;
	}
