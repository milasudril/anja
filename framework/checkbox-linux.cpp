#ifdef __WAND__
target
	[
	name[checkbox.o]
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

#include "checkbox.h"
#include "guicontainer.h"
#include "guihandle.h"
#include "exceptionswallow.h"

class CheckboxGtk:public Checkbox
	{
	public:
		CheckboxGtk(GuiContainer& parent,EventHandler& handler,unsigned int id
			,const char* title);
		~CheckboxGtk();

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_checkbox;}

		bool stateGet() const
			{
			GtkWidget* handle=m_checkbox;
			return gtk_toggle_button_get_active((GtkToggleButton*)handle);
			}

		void stateSet(bool state_new)
			{
			GtkWidget* handle=m_checkbox;
			return gtk_toggle_button_set_active((GtkToggleButton*)handle,state_new);
			}

		unsigned int idGet() const
			{return m_id;}

	private:
		static void onClick(GtkWidget* widget,void* checkboxgtk);

		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_checkbox;
		unsigned int m_id;
	};

void CheckboxGtk::onClick(GtkWidget* widget,void* checkboxgtk)
	{
	CheckboxGtk* _this=reinterpret_cast<CheckboxGtk*>(checkboxgtk);
	EXCEPTION_SWALLOW(
		if(_this->stateGet())
			{_this->r_handler->onSet(*_this);}
		else
			{_this->r_handler->onUnset(*_this);}
	,_this);
	}

Checkbox::EventHandler Checkbox::s_default_handler;

Checkbox* Checkbox::create(GuiContainer& parent,EventHandler& handler
	,unsigned int id,const char* title)
	{return new CheckboxGtk(parent,handler,id,title);}

void CheckboxGtk::destroy()
	{delete this;}

CheckboxGtk::CheckboxGtk(GuiContainer& parent,EventHandler& handler
	,unsigned int id,const char* title):
	r_parent(parent),r_handler(&handler),m_id(id)
	{
	GtkWidget* widget=gtk_check_button_new_with_label(title);
	g_signal_connect(widget,"clicked",G_CALLBACK(onClick),this);
	m_checkbox=widget;
	g_object_ref_sink(widget);
	parent.componentAdd(*this);
	}

CheckboxGtk::~CheckboxGtk()
	{
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_checkbox);
	}
