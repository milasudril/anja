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
		ButtonGtk(GuiContainer& parent,const char* title,unsigned int command_id);
		~ButtonGtk();

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_button;}

	private:
		static void onClick(GtkWidget* widget,void* buttongtk);

		GuiContainer& r_parent;
		GuiHandle m_button;
		unsigned int m_command_id;
	};

void ButtonGtk::onClick(GtkWidget* widget,void* buttongtk)
	{
	ButtonGtk* _this=(ButtonGtk*)buttongtk;
	EXCEPTION_SWALLOW(_this->r_parent.commandNotify(_this->m_command_id);
		,_this);
	}

Button* Button::create(GuiContainer& parent,const char* title
	,unsigned int command_id)
	{return new ButtonGtk(parent,title,command_id);}

void ButtonGtk::destroy()
	{delete this;}

ButtonGtk::ButtonGtk(GuiContainer& parent,const char* title,unsigned int command_id):
	r_parent(parent),m_command_id(command_id)
	{
	GtkWidget* widget=gtk_button_new_with_label(title);
	g_signal_connect(widget,"clicked",G_CALLBACK(onClick),this);
	m_button=widget;
	g_object_ref(widget);
	parent.componentAdd(*this);
	}

ButtonGtk::~ButtonGtk()
	{
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_button);
	}
