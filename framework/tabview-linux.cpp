#ifdef __WAND__
target
	[
	name[tabview.o]
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

#include "tabview.h"
#include "guihandle.h"
#include "arraydynamicshort.h"
#include <algorithm>

class TabViewGtk:public TabView
	{
	public:
		TabViewGtk(GuiContainer& parent,EventHandler* event_handler);
		~TabViewGtk();

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_box;}

		void componentAdd(Widget& widget);
		void componentRemove(Widget& widget);

		void slaveAssign(Widget& widget)
			{r_slave=&widget;}

		void slaveRelease()
			{r_slave=nullptr;}

		void tabTitleSet(unsigned int tab,const char* title)
			{
			GtkWidget* handle=m_box;
			auto widget=gtk_notebook_get_nth_page((GtkNotebook*)handle,tab);
			gtk_notebook_set_tab_label_text((GtkNotebook*)handle,widget,title);
			}

	private:
		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_box;
		Widget* r_slave;

		ArrayDynamicShort<Widget*> m_widgets;
		void componentRemoveAt(const ArrayDynamicShort<Widget*>::iterator& i);
	};

TabView* TabView::create(GuiContainer& parent,EventHandler* handler)
	{return new TabViewGtk(parent,handler);}

void TabViewGtk::destroy()
	{delete this;}

TabViewGtk::TabViewGtk(GuiContainer& parent,EventHandler* handler):
	r_parent(parent),r_handler(handler),r_slave(nullptr)
	{
	GtkWidget* box=gtk_notebook_new();
	m_box=box;
	g_object_ref_sink(box);
	r_parent.componentAdd(*this);
	gtk_widget_show(box);
	}

TabViewGtk::~TabViewGtk()
	{
	if(r_slave!=nullptr)
		{r_slave->destroy();}

	std::for_each(m_widgets.rbegin(),m_widgets.rend(),widgetDestroy);
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_box);
	}

void TabViewGtk::componentAdd(Widget& widget)
	{
	auto h=widget.handleNativeGet();
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,&widget);
	if(i!=m_widgets.end())
		{return;}
	m_widgets.append(&widget);
	GtkWidget* box=m_box;
	gtk_notebook_append_page((GtkNotebook*)box,h,NULL);
	gtk_widget_show(h);
	}

void TabViewGtk::componentRemove(Widget& widget)
	{
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,&widget);
	if(i!=m_widgets.end())
		{
		componentRemoveAt(i);
		}
	}

void TabViewGtk::componentRemoveAt(const ArrayDynamicShort<Widget*>::iterator& i)
	{
	GtkWidget* widget=m_box;
	auto object=*i;
	auto handle=object->handleNativeGet();
	gtk_container_remove(GTK_CONTAINER(widget),handle);
	m_widgets.erase(i);
	}
