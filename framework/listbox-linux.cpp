#ifdef __WAND__
target
	[
	name[listbox.o]
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

#include "listbox.h"
#include "guicontainer.h"
#include "guihandle.h"
#include "exceptionswallow.h"

class ListboxGtk:public Listbox
	{
	public:
		ListboxGtk(GuiContainer& parent,EventHandler& handler,unsigned int id);
		~ListboxGtk();
		void destroy();
		const GuiHandle& handleNativeGet() const
			{return m_listbox;}

		unsigned int idGet() const
			{return m_id;}

		void optionAppend(const char* option)
			{
			GtkWidget* widget=m_listbox;
			gtk_combo_box_text_append_text((GtkComboBoxText*)widget,option);
			}

		void optionReplace(unsigned int index,const char* option)
			{
			GtkWidget* widget=m_listbox;
			auto i=optionSelectedGet();
			gtk_combo_box_text_remove((GtkComboBoxText*)widget,index);
			gtk_combo_box_text_insert((GtkComboBoxText*)widget,index,NULL,option);
			optionSelect(i);
			}

		void optionSelect(unsigned int index)
			{
			GtkWidget* widget=m_listbox;
			gtk_combo_box_set_active ((GtkComboBox*)widget,index);
			}

		unsigned int optionSelectedGet() const
			{
			GtkWidget* widget=m_listbox;
			return gtk_combo_box_get_active((GtkComboBox*)widget);
			}

		void optionsClear()
			{
			GtkWidget* widget=m_listbox;
			g_signal_handlers_disconnect_by_func(widget, (void*)onOptionSelect, this);
			gtk_combo_box_text_remove_all((GtkComboBoxText*)widget);
			g_signal_connect(widget,"changed",G_CALLBACK(onOptionSelect),this);
			}


	private:
		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_listbox;
		unsigned int m_id;

		static void onOptionSelect(GtkComboBox* widget,void* listboxgtk)
			{
			ListboxGtk* _this=reinterpret_cast<ListboxGtk*>(listboxgtk);

			EXCEPTION_SWALLOW(_this->r_handler->onOptionSelect(*_this);
				,_this);
			}
	};

Listbox::EventHandler Listbox::s_default_handler;

Listbox* Listbox::create(GuiContainer& parent,EventHandler& handler
	,unsigned int id)
	{return new ListboxGtk(parent,handler,id);}

void ListboxGtk::destroy()
	{delete this;}

ListboxGtk::ListboxGtk(GuiContainer& parent,EventHandler& handler
	,unsigned int id):r_parent(parent),r_handler(&handler),m_id(id)
	{
	GtkWidget* widget=gtk_combo_box_text_new();
	m_listbox=widget;
	g_signal_connect(widget,"changed",G_CALLBACK(onOptionSelect),this);
	g_object_ref_sink(widget);
	parent.componentAdd(*this);
	}

ListboxGtk::~ListboxGtk()
	{
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_listbox);
	}
