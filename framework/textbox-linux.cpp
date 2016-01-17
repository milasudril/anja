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
#include "exceptionswallow.h"

class TextboxGtk:public Textbox
	{
	public:
		TextboxGtk(GuiContainer& parent,unsigned int element_id);
		~TextboxGtk();
		void destroy();

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

		void widthMinSet(unsigned int nChars)
			{
			GtkWidget* widget=m_textbox;
			gtk_entry_set_width_chars((GtkEntry*)widget,nChars);
			}

	private:
		static gboolean onBlur(GtkWidget* entry,GdkEvent* event,void* textboxgtk);

		GuiContainer& r_parent;
		GuiHandle m_textbox;
		unsigned int m_element_id;
	};

gboolean TextboxGtk::onBlur(GtkWidget* entry,GdkEvent* event,void* textboxgtk)
	{
	TextboxGtk* _this=(TextboxGtk*)textboxgtk;
	EXCEPTION_SWALLOW(_this->r_parent.commandNotify(_this->m_element_id);
		,_this);
	return FALSE;
	}

Textbox* Textbox::create(GuiContainer& parent,unsigned int element_id)
	{return new TextboxGtk(parent,element_id);}

void TextboxGtk::destroy()
	{delete this;}

TextboxGtk::TextboxGtk(GuiContainer& parent,unsigned int element_id):
	r_parent(parent),m_element_id(element_id)
	{
	GtkWidget* widget=gtk_entry_new();
	gtk_widget_add_events(widget,GDK_FOCUS_CHANGE_MASK);

	g_signal_connect(widget,"focus-out-event",G_CALLBACK(onBlur),this);
	m_textbox=widget;
	g_object_ref_sink(widget);
	parent.componentAdd(*this);
	}

TextboxGtk::~TextboxGtk()
	{
	GtkWidget* w=m_textbox;
	g_signal_handlers_disconnect_by_func(w, (void*)onBlur, this);
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_textbox);
	}
