#ifdef __WAND__
target
	[
	name[window.o]
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

#include "window.h"
#include "guihandle.h"

class WindowGtk:public Window
	{
	public:
		WindowGtk(EventLoop& event_loop,EventHandler& handler,WindowGtk* owner);
		~WindowGtk();

		void destroy()
			{gtk_widget_destroy(m_window);}

		void eventHandlerSet(EventHandler& handler)
			{r_handler=&handler;}

		void componentAdd(const GuiHandle& component);
		void componentRemove(const GuiHandle& component);
		void titleSet(const char* title_new)
			{gtk_window_set_title((GtkWindow*)m_window,title_new);}

	private:
		static gboolean onClose(GtkWidget* object,GdkEvent* event,void* windowgtk);

		static gboolean onMouseMove(GtkWidget* object,GdkEventMotion* event
			,void* windowgtk);

		static gboolean onMouseDown(GtkWidget* object,GdkEventButton* event
			,void* windowgtk);

		static gboolean onMouseUp(GtkWidget* object,GdkEventButton* event
			,void* windowgtk);

		static gboolean onKeyDown(GtkWidget* widget,GdkEventKey* event
			,void* windowgtk);

		static gboolean onKeyUp(GtkWidget* widget,GdkEventKey* event
			,void* windowgtk);

		static void onDestroy(GtkWidget* widget,void* windowgtk);

		EventHandler* r_handler;
		GtkWidget* r_widget;
		GtkWidget* m_window;
	};

Window::EventHandler Window::s_null_handler;

Window* Window::create(EventLoop& event_loop,EventHandler& handler,Window* owner)
	{return new WindowGtk(event_loop,handler,dynamic_cast<WindowGtk*>(owner));}

gboolean WindowGtk::onClose(GtkWidget* widget,GdkEvent* event,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	if(_this->r_handler->onClose())
		{_this->destroy();}
	return TRUE;
	}

gboolean WindowGtk::onMouseMove(GtkWidget* object,GdkEventMotion* event
	,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	_this->r_handler->onMouseMove(event->x,event->y,keymaskFromSystem(event->state));
	return TRUE;
	}

gboolean WindowGtk::onMouseDown(GtkWidget* object,GdkEventButton* event
	,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	_this->r_handler->onMouseDown(event->x,event->y,keymaskFromSystem(event->state));
	return TRUE;
	}

gboolean WindowGtk::onMouseUp(GtkWidget* object,GdkEventButton* event
	,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	_this->r_handler->onMouseUp(event->x,event->y,keymaskFromSystem(event->state));
	return TRUE;
	}

gboolean WindowGtk::onKeyDown(GtkWidget *widget, GdkEventKey *event
	,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	_this->r_handler->onKeyDown(keymaskFromSystem(event->state));
	return FALSE;
	}

gboolean WindowGtk::onKeyUp(GtkWidget *widget, GdkEventKey *event
	,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	_this->r_handler->onKeyDown(keymaskFromSystem(event->state));
	return FALSE;
	}

void WindowGtk::onDestroy(GtkWidget* widget,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	delete _this;
	}

WindowGtk::WindowGtk(EventLoop& event_loop,EventHandler& handler,WindowGtk* owner):
	Window(event_loop),r_handler(&handler),r_widget(nullptr)
	{
	m_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_object_ref_sink(G_OBJECT(m_window));
	gtk_widget_show(m_window);
	g_signal_connect(m_window,"button-release-event",G_CALLBACK(onMouseUp),this);
	g_signal_connect(m_window,"button-press-event",G_CALLBACK(onMouseDown),this);
	g_signal_connect(m_window,"motion-notify-event",G_CALLBACK(onMouseMove),this);
	g_signal_connect(m_window,"key_press_event",G_CALLBACK(onKeyDown),this);
	g_signal_connect(m_window,"key_release_event",G_CALLBACK(onKeyUp),this);
	g_signal_connect(m_window,"delete-event",G_CALLBACK(onClose),this);
	g_signal_connect(m_window,"destroy",G_CALLBACK(onDestroy),this);
	if(owner!=nullptr)
		{
		gtk_window_set_transient_for((GtkWindow*)m_window,(GtkWindow*)(owner->m_window));
		gtk_window_set_destroy_with_parent((GtkWindow*)m_window,TRUE);
		}
	}

WindowGtk::~WindowGtk()
	{
	}

void WindowGtk::componentAdd(const GuiHandle& component)
	{
	if(r_widget!=nullptr)
		{componentRemove(r_widget);}
	r_widget=component;
	gtk_container_add(GTK_CONTAINER(m_window),r_widget);
	gtk_widget_show(r_widget);
	}

void WindowGtk::componentRemove(const GuiHandle& component)
	{
	gtk_widget_hide(r_widget);
	gtk_container_remove(GTK_CONTAINER(m_window),r_widget);
	r_widget=nullptr;
	}
