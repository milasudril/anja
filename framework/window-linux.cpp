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
#include "exceptionswallow.h"
#include "arraydynamicshort.h"
#include <algorithm>

class WindowGtk:public Window
	{
	public:
		WindowGtk(EventLoop& event_loop,EventHandler& handler,WindowGtk* owner);
		~WindowGtk();

		void destroy();

		void eventHandlerSet(EventHandler& handler)
			{r_handler=&handler;}

		void componentAdd(Widget& component);
		void componentRemove(Widget& component);

		void slaveAssign(Widget& component)
			{r_slave=&component;}

		void slaveRelease()
			{r_slave=nullptr;}

		void titleSet(const char* title_new)
			{
			GtkWidget* window=m_window;
			gtk_window_set_title((GtkWindow*)window,title_new);
			}

		const GuiHandle& handleNativeGet() const
			{return m_window;}

		void commandNotify(unsigned int command_id)
			{r_handler->onCommand(*this,command_id);}

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

		void windowRemove(WindowGtk& window);

		EventHandler* r_handler;
		Widget* r_widget;
		Widget* r_slave;
		WindowGtk* r_owner;
		ArrayDynamicShort<WindowGtk*> m_owned_windows;
		GuiHandle m_window;
	};

Window::EventHandler Window::s_default_handler;

Window* Window::create(EventLoop& event_loop,EventHandler& handler,Window* owner)
	{return new WindowGtk(event_loop,handler,dynamic_cast<WindowGtk*>(owner));}

void WindowGtk::destroy()
	{delete this;}

gboolean WindowGtk::onClose(GtkWidget* widget,GdkEvent* event,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	EXCEPTION_SWALLOW(_this->r_handler->onClose(*_this);,_this);
	return TRUE;
	}

gboolean WindowGtk::onMouseMove(GtkWidget* object,GdkEventMotion* event
	,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	EXCEPTION_SWALLOW(
		_this->r_handler->onMouseMove(*_this,event->x,event->y,keymaskFromSystem(event->state));
		,_this);
	return TRUE;
	}

gboolean WindowGtk::onMouseDown(GtkWidget* widget,GdkEventButton* event
	,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	EXCEPTION_SWALLOW(
		_this->r_handler->onMouseDown(*_this,event->x,event->y,keymaskFromSystem(event->state));
		,_this);
	return TRUE;
	}

gboolean WindowGtk::onMouseUp(GtkWidget* widget,GdkEventButton* event
	,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	EXCEPTION_SWALLOW(
		_this->r_handler->onMouseUp(*_this,event->x,event->y,keymaskFromSystem(event->state));
		,_this);
	return TRUE;
	}

gboolean WindowGtk::onKeyDown(GtkWidget* widget,GdkEventKey* event
	,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	EXCEPTION_SWALLOW(_this->r_handler->onKeyDown(*_this,keymaskFromSystem(event->state));
		,_this);
	return FALSE;
	}

gboolean WindowGtk::onKeyUp(GtkWidget* widget,GdkEventKey* event
	,void* windowgtk)
	{
	WindowGtk* _this=(WindowGtk*)windowgtk;
	EXCEPTION_SWALLOW(_this->r_handler->onKeyDown(*_this,keymaskFromSystem(event->state));
		,_this);
	return FALSE;
	}

WindowGtk::WindowGtk(EventLoop& event_loop,EventHandler& handler,WindowGtk* owner):
	Window(event_loop),r_handler(&handler),r_widget(nullptr)
	,r_slave(nullptr),r_owner(owner)
	{
	GtkWidget* window_this=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_object_ref_sink(G_OBJECT(window_this));
	gtk_widget_show(window_this);
	g_signal_connect(window_this,"button-release-event",G_CALLBACK(onMouseUp),this);
	g_signal_connect(window_this,"button-press-event",G_CALLBACK(onMouseDown),this);
	g_signal_connect(window_this,"motion-notify-event",G_CALLBACK(onMouseMove),this);
	g_signal_connect(window_this,"key_press_event",G_CALLBACK(onKeyDown),this);
	g_signal_connect(window_this,"key_release_event",G_CALLBACK(onKeyUp),this);
	g_signal_connect(window_this,"delete-event",G_CALLBACK(onClose),this);
	if(owner!=nullptr)
		{
		GtkWidget* window_parent=owner->m_window;
		gtk_window_set_transient_for((GtkWindow*)window_this
			,(GtkWindow*)window_parent);
		owner->m_owned_windows.append(this);
		}
	m_window=window_this;
	}

void WindowGtk::windowRemove(WindowGtk& window)
	{
	auto begin=m_owned_windows.begin();
	auto end=m_owned_windows.end();
	auto i=std::find(begin,end,&window);
	if(i!=m_owned_windows.end())
		{m_owned_windows.erase(i);}
	}

WindowGtk::~WindowGtk()
	{
	if(r_slave!=nullptr)
		{r_slave->destroy();}
	if(r_widget!=nullptr)
		{r_widget->destroy();}
	std::for_each(m_owned_windows.rbegin(),m_owned_windows.rend(),widgetDestroy);
	if(r_owner!=nullptr)
		{r_owner->windowRemove(*this);}
	gtk_widget_destroy(m_window);
	}


void WindowGtk::componentAdd(Widget& component)
	{
	if(r_widget==&component)
		{return;}
	if(r_widget!=nullptr)
		{componentRemove(*r_widget);}
	r_widget=&component;
	auto handle=component.handleNativeGet();
	GtkWidget* window=m_window;
	gtk_container_add(GTK_CONTAINER(window),handle);
	gtk_widget_show(handle);
	}

void WindowGtk::componentRemove(Widget& component)
	{
	auto handle=r_widget->handleNativeGet();
	GtkWidget* window=m_window;
	gtk_container_remove(GTK_CONTAINER(window),handle);
	r_widget=nullptr;
	}
