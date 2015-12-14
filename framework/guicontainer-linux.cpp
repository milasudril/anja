#ifdef __WAND__
target
	[
	name[guicontainer.o]
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

#include "guicontainer.h"
#include <gtk/gtk.h>

class GuiContainer::Impl
	{
	public:
		Impl(GuiContainer& me, EventHandler& handler):r_handler(handler)
			{
			m_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
			gtk_widget_show(m_window);
			g_object_ref_sink(G_OBJECT(m_window));
			g_signal_connect(m_window,"key_press_event",G_CALLBACK(keydown),this);
			g_signal_connect(m_window,"key_release_event",G_CALLBACK(keyup),this);
			g_signal_connect(m_window,"delete-event",G_CALLBACK(destroy),&me);
			++s_windowcount;
			}

		~Impl()
			{
			gtk_container_remove((GtkContainer*)m_window,m_widget);
			gtk_widget_destroy(m_window);
			g_object_unref(m_window);
			--s_windowcount;
			if(s_windowcount==0)
				{gtk_main_quit();}
			}

		void objectAdd(void* handle_native)
			{
			gtk_container_add (GTK_CONTAINER(m_window),(GtkWidget*)handle_native);
			gtk_widget_show((GtkWidget*)handle_native);
			m_widget=(GtkWidget*)handle_native;
			}

	private:
		static size_t s_windowcount;
		static gboolean destroy(GtkWidget* widget,GdkEvent* event,void* user_data)
			{
			GuiContainer* me=(GuiContainer*)user_data;
 			delete me;
			return TRUE;
			}

		static gboolean keydown(GtkWidget *widget, GdkEventKey *event,void* impl)
			{
			Impl* _this=(Impl*)impl;
			_this->r_handler.keyDown(event->hardware_keycode-8);
			return 0;
			}

		static gboolean keyup(GtkWidget *widget, GdkEventKey *event,void* impl)
			{
			Impl* _this=(Impl*)impl;
			_this->r_handler.keyUp(event->hardware_keycode-8);
			return 0;
			}

		EventHandler& r_handler;

		GtkWidget* m_window;
		GtkWidget* m_widget;
	};

size_t GuiContainer::Impl::s_windowcount=0;

GuiContainer::GuiContainer(EventHandler& handler)
	{
	m_impl=new Impl(*this,handler);
	}

void GuiContainer::objectAdd(void* handle_native)
	{
	m_impl->objectAdd(handle_native);
	}

GuiContainer::~GuiContainer()
	{
	delete m_impl;
	}
