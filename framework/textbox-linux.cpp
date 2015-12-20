#ifdef __WAND__
target
	[
	name[textbox.o]
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

#include "textbox.h"
#include "guicontainer.h"
#include "guihandle.h"

class TextboxGtk:public Textbox
	{
	public:
		TextboxGtk(GuiContainer& parent,unsigned int element_id);
		void destroy()
			{r_parent.componentRemove(*this);}

		const GuiHandle& handleNativeGet() const
			{return m_textbox;}

		const char* textGet() const
			{
			GtkWidget* widget=m_textbox;
			return gtk_entry_get_text((GtkEntry*)widget);
			}

		void textSet(const char* text)
			{
			GtkWidget* widget=m_textbox;
			return gtk_entry_set_text((GtkEntry*)widget,text);
			}

	private:
		static void onDestroy(GtkWidget* widget,void* textboxgtk);
		static gboolean onKeyUp(GtkWidget* entry,GdkEvent* event,void* textboxgtk);

		GuiContainer& r_parent;
		GuiHandle m_textbox;
		unsigned int m_element_id;
	};

void TextboxGtk::onDestroy(GtkWidget* widget,void* textboxgtk)
	{
	TextboxGtk* _this=(TextboxGtk*)textboxgtk;
	delete _this;
	}

gboolean TextboxGtk::onKeyUp(GtkWidget* entry,GdkEvent* event,void* textboxgtk)
	{
	TextboxGtk* _this=(TextboxGtk*)textboxgtk;
	_this->r_parent.commandNotify(_this->m_element_id);
	return TRUE;
	}

Textbox* Textbox::create(GuiContainer& parent,unsigned int element_id)
	{return new TextboxGtk(parent,element_id);}

TextboxGtk::TextboxGtk(GuiContainer& parent,unsigned int element_id):
	r_parent(parent),m_element_id(element_id)
	{
	GtkWidget* widget=gtk_entry_new();
	g_signal_connect(widget,"key_release_event",G_CALLBACK(onKeyUp),this);
	g_signal_connect(widget,"destroy",G_CALLBACK(onDestroy),this);
	m_textbox=widget;
	parent.componentAdd(*this);
	}
