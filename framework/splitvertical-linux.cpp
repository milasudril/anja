#ifdef __WAND__
target
	[
	name[splitvertical.o]
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

#include "splitvertical.h"
#include "guihandle.h"
#include "arraydynamicshort.h"
#include <algorithm>

class SplitVerticalGtk:public SplitVertical
	{
	public:
		SplitVerticalGtk(GuiContainer& parent,EventHandler* event_handler);
		~SplitVerticalGtk();

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_split;}

		void componentAdd(Widget& widget);
		void componentRemove(Widget& widget);

		void slaveAssign(Widget& slave)
			{r_slave=&slave;}

		void slaveRelease()
			{r_slave=nullptr;}

		void commandNotify(unsigned int command_id)
			{
			if(r_handler==nullptr)
				{r_parent.commandNotify(command_id);}
			else
				{r_handler->onCommand(*this,command_id);}
			}

	private:
		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_split;
		Widget* r_slave;
		uint32_t m_insert_mode;

		ArrayDynamicShort<Widget*> m_widgets;
		void componentRemoveAt(const ArrayDynamicShort<Widget*>::iterator& i);
	};

SplitVertical* SplitVertical::create(GuiContainer& parent,EventHandler* handler)
	{return new SplitVerticalGtk(parent,handler);}

void SplitVerticalGtk::destroy()
	{delete this;}

SplitVerticalGtk::SplitVerticalGtk(GuiContainer& parent,EventHandler* handler):
	r_parent(parent),r_handler(handler),r_slave(nullptr),m_insert_mode(0)
	{
	GtkWidget* split=gtk_paned_new(GTK_ORIENTATION_VERTICAL);
	m_split=split;
	g_object_ref_sink(split);
	r_parent.componentAdd(*this);
	gtk_widget_set_size_request (split, -1, 400);
	gtk_paned_set_position((GtkPaned*)split,200);
	gtk_widget_show(split);
	}

SplitVerticalGtk::~SplitVerticalGtk()
	{
	if(r_slave!=nullptr)
		{r_slave->destroy();}
	std::for_each(m_widgets.rbegin(),m_widgets.rend(),widgetDestroy);
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_split);
	}

void SplitVerticalGtk::componentAdd(Widget& widget)
	{
	auto h=widget.handleNativeGet();
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,&widget);
	if(i!=m_widgets.end())
		{return;}

	m_widgets.append(&widget);

	GtkWidget* split=m_split;
	if(m_widgets.length()==0)
		{gtk_paned_add1((GtkPaned*)split,h);}
	else
		{gtk_paned_add2((GtkPaned*)split,h);}
	gtk_widget_set_size_request(h,-1, 100);
	gtk_widget_show(h);
	}

void SplitVerticalGtk::componentRemove(Widget& widget)
	{
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,&widget);
	if(i!=m_widgets.end())
		{
		componentRemoveAt(i);
		}
	}

void SplitVerticalGtk::componentRemoveAt(const ArrayDynamicShort<Widget*>::iterator& i)
	{
	GtkWidget* widget=m_split;
	auto object=*i;
	auto handle=object->handleNativeGet();
	gtk_container_remove(GTK_CONTAINER(widget),handle);
	m_widgets.erase(i);
	}
