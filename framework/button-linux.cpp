#ifdef __WAND__
target
	[
	name[button.o]
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

#include "button.h"
#include "guicontainer.h"
#include "guihandle.h"
#include "exceptionswallow.h"

class ButtonGtk:public Button
	{
	public:
		ButtonGtk(GuiContainer& parent,EventHandler& handler,unsigned int id
			,const char* title);
		~ButtonGtk();

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_button;}

		unsigned int idGet() const
			{return m_id;}

		void titleSet(const char* title)
			{
			GtkWidget* widget=m_button;
			gtk_button_set_label((GtkButton*)widget,title);
			}

	private:
		static void onClick(GtkWidget* widget,void* buttongtk);

		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_button;
		unsigned int m_id;
	};

void ButtonGtk::onClick(GtkWidget* widget,void* buttongtk)
	{
	ButtonGtk* _this=reinterpret_cast<ButtonGtk*>(buttongtk);
	EXCEPTION_SWALLOW(_this->r_handler->onActionPerform(*_this);
		,_this);
	}

Button* Button::create(GuiContainer& parent,EventHandler& handler
	,unsigned int id,const char* title)
	{return new ButtonGtk(parent,handler,id,title);}

Button::EventHandler Button::s_default_handler;

void ButtonGtk::destroy()
	{delete this;}

ButtonGtk::ButtonGtk(GuiContainer& parent,EventHandler& handler
	,unsigned int id,const char* title):
	r_parent(parent),r_handler(&handler),m_id(id)
	{
	GtkWidget* widget=gtk_button_new_with_label(title);
	g_signal_connect(widget,"clicked",G_CALLBACK(onClick),this);
	m_button=widget;
	g_object_ref_sink(widget);
	parent.componentAdd(*this);
	}

ButtonGtk::~ButtonGtk()
	{
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_button);
	}
