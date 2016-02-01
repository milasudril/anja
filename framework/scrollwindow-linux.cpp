#ifdef __WAND__
target
	[
	name[scrollwindow.o]
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

#include "scrollwindow.h"
#include "guihandle.h"
#include "exceptionswallow.h"
#include "arraydynamicshort.h"
#include <algorithm>

class ScrollWindowGtk:public ScrollWindow
	{
	public:
		ScrollWindowGtk(GuiContainer& parent,EventHandler& handler
			,unsigned int mode);
		~ScrollWindowGtk();

		void destroy();

		void eventHandlerSet(EventHandler& handler)
			{r_handler=&handler;}

		void componentAdd(Widget& component);
		void componentRemove(Widget& component);

		void slaveAssign(Widget& component)
			{r_slave=&component;}

		void slaveRelease()
			{r_slave=nullptr;}

		const GuiHandle& handleNativeGet() const
			{return m_box;}

		void fullscreenSet(bool state)
			{
			GtkWidget* window=gtk_widget_get_toplevel(m_box);
			if(gtk_widget_is_toplevel(window))
				{
				if(state)
					{gtk_window_fullscreen((GtkWindow*)window);}
				else
					{gtk_window_unfullscreen((GtkWindow*)window);}
				}
			}

		void widthMinSet(unsigned int w)
			{
			GtkWidget* box=m_box;
			gtk_scrolled_window_set_min_content_width((GtkScrolledWindow*)box,w);
			}

		void heightMinSet(unsigned int h)
			{
			GtkWidget* box=m_box;
			gtk_scrolled_window_set_min_content_height((GtkScrolledWindow*)box,h);
			}


	private:
		GuiContainer& r_parent;
		EventLoop* r_event_loop;
		EventHandler* r_handler;
		Widget* r_widget;
		Widget* r_slave;
		GuiHandle m_box;
	};

ScrollWindow::EventHandler ScrollWindow::s_default_handler;

ScrollWindow* ScrollWindow::create(GuiContainer& parent,EventHandler& handler
	,unsigned int mode)
	{return new ScrollWindowGtk(parent,handler,mode);}

void ScrollWindowGtk::destroy()
	{
	if(r_slave==nullptr)
		{delete this;}
	else
		{r_slave->destroy();}
	}

ScrollWindowGtk::ScrollWindowGtk(GuiContainer& parent,EventHandler& handler
	,unsigned int mode):
	r_parent(parent),r_handler(&handler),r_widget(nullptr),r_slave(nullptr)
	{
	GtkWidget* box=gtk_scrolled_window_new(NULL,NULL);
	m_box=box;
	g_object_ref_sink(box);
	r_parent.componentAdd(*this);
	gtk_widget_show(box);

	switch(mode)
		{
		case 0:
			gtk_scrolled_window_set_policy((GtkScrolledWindow*)box
				,GTK_POLICY_NEVER,GTK_POLICY_NEVER);
			break;
		case 1:
			gtk_scrolled_window_set_policy((GtkScrolledWindow*)box
				,GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
			break;
		case 2:
			gtk_scrolled_window_set_policy((GtkScrolledWindow*)box
				,GTK_POLICY_AUTOMATIC,GTK_POLICY_NEVER);
			break;
		case 3:
			gtk_scrolled_window_set_policy((GtkScrolledWindow*)box
				,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
			break;
		}
	}

ScrollWindowGtk::~ScrollWindowGtk()
	{
	if(r_slave!=nullptr)
		{r_slave->destroy();}
	if(r_widget!=nullptr)
		{r_widget->destroy();}
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_box);
	}


void ScrollWindowGtk::componentAdd(Widget& component)
	{
	if(r_widget==&component)
		{return;}
	if(r_widget!=nullptr)
		{componentRemove(*r_widget);}
	r_widget=&component;
	auto handle=component.handleNativeGet();
	GtkWidget* window=m_box;
	gtk_container_add(GTK_CONTAINER(window),handle);
	gtk_widget_show(handle);
	}

void ScrollWindowGtk::componentRemove(Widget& component)
	{
	auto handle=r_widget->handleNativeGet();
	GtkWidget* window=m_box;
	auto parent=gtk_widget_get_parent(handle);
	if(parent!=window)
		{
		gtk_container_remove(GTK_CONTAINER(parent),handle);
		gtk_container_remove(GTK_CONTAINER(window),parent);
		}
	else
		{gtk_container_remove(GTK_CONTAINER(window),handle);}

	r_widget=nullptr;
	}
