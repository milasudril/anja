#ifdef __WAND__
target
	[
	name[messagedisplay.o]
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

#include "messagedisplay.h"
#include "widget.h"
#include "guihandle.h"
#include <gtk/gtk.h>

GtkMessageType messageTypeConvert(unsigned int message_type)
	{
	auto v=message_type&0xf0;
	switch(v)
		{
		case MESSAGE_TYPE_NORMAL:
			return GTK_MESSAGE_INFO;
		case MESSAGE_TYPE_WARNING:
			return GTK_MESSAGE_WARNING;
		case MESSAGE_TYPE_ERROR:
			return GTK_MESSAGE_ERROR;
		default:
			return GTK_MESSAGE_OTHER;
		}
	}

void buttonsAdd(GtkDialog* dialog,unsigned int message_type)
	{
	auto v=message_type&0xf;
	switch(v)
		{
		case MESSAGE_ANSWERS_OK:
			gtk_dialog_add_buttons(dialog,"Ok",MESSAGE_ANSWER_OK,NULL);
			break;
		case MESSAGE_ANSWERS_RETRYCANCEL:
			gtk_dialog_add_buttons(dialog
				,"Retry",MESSAGE_ANSWER_RETRY
				,"Cancel",MESSAGE_ANSWER_CANCEL,NULL);
			break;
		case MESSAGE_ANSWERS_YESNO:
			gtk_dialog_add_buttons(dialog
				,"Yes",MESSAGE_ANSWER_YES
				,"No",MESSAGE_ANSWER_NO
				,NULL);
			break;
		case MESSAGE_ANSWERS_YESNOCANCEL:
			gtk_dialog_add_buttons(dialog
				,"Yes",MESSAGE_ANSWER_YES
				,"No",MESSAGE_ANSWER_NO
				,"Cancel",MESSAGE_ANSWER_CANCEL
				,NULL);
			break;
		}
	}


uint32_t messageDisplay(const Widget& owner,const char* message
	,const char* context_title
	,unsigned int message_type)
	{
	auto parent=gtk_widget_get_toplevel(owner.handleNativeGet());
	auto flags = GTK_DIALOG_DESTROY_WITH_PARENT;

	auto message_type_gtk=messageTypeConvert(message_type);

	auto dialog=gtk_message_dialog_new((GtkWindow*)parent,flags,message_type_gtk
		,GTK_BUTTONS_NONE,"%s",message);

	buttonsAdd((GtkDialog*)dialog,message_type);
	gtk_window_set_title(GTK_WINDOW(dialog),context_title);

	auto result=gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);

	return result;
	}
