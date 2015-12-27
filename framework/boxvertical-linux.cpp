#ifdef __WAND__
target
	[
	name[boxvertical.o]
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

#include "boxvertical.h"
#include "guihandle.h"
#include "arraydynamicshort.h"
#include <algorithm>

class BoxVerticalGtk:public BoxVertical
	{
	public:
		BoxVerticalGtk(GuiContainer& parent,EventHandler* event_handler);
		~BoxVerticalGtk();

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_box;}

		void insertModeSet(uint32_t mode)
			{m_insert_mode=mode;}

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
		GuiHandle m_box;
		Widget* r_slave;
		uint32_t m_insert_mode;

		ArrayDynamicShort<Widget*> m_widgets;
		void componentRemoveAt(const ArrayDynamicShort<Widget*>::iterator& i);
	};

BoxVertical* BoxVertical::create(GuiContainer& parent,EventHandler* handler)
	{return new BoxVerticalGtk(parent,handler);}

void BoxVerticalGtk::destroy()
	{delete this;}

BoxVerticalGtk::BoxVerticalGtk(GuiContainer& parent,EventHandler* handler):
	r_parent(parent),r_handler(handler),r_slave(nullptr),m_insert_mode(0)
	{
	GtkWidget* box=gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
	m_box=box;
	g_object_ref_sink(box);
	r_parent.componentAdd(*this);
	gtk_widget_show(box);
	}

BoxVerticalGtk::~BoxVerticalGtk()
	{
	if(r_slave!=nullptr)
		{r_slave->destroy();}
	std::for_each(m_widgets.rbegin(),m_widgets.rend(),widgetDestroy);
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_box);
	}

void BoxVerticalGtk::componentAdd(Widget& widget)
	{
	auto h=widget.handleNativeGet();
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,&widget);
	if(i!=m_widgets.end())
		{return;}

	m_widgets.append(&widget);
	auto insert_mode=m_insert_mode;
	GtkWidget* box=m_box;
	if(insert_mode&INSERTMODE_END)
		{
		gtk_box_pack_end((GtkBox*)box,h,insert_mode&INSERTMODE_EXPAND
			,insert_mode&INSERTMODE_FILL,0);
		}
	else
		{
		gtk_box_pack_start((GtkBox*)box,h,insert_mode&INSERTMODE_EXPAND
			,insert_mode&INSERTMODE_FILL,0);
		}
	gtk_widget_show(h);
	}

void BoxVerticalGtk::componentRemove(Widget& widget)
	{
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,&widget);
	if(i!=m_widgets.end())
		{
		componentRemoveAt(i);
		}
	}

void BoxVerticalGtk::componentRemoveAt(const ArrayDynamicShort<Widget*>::iterator& i)
	{
	GtkWidget* widget=m_box;
	auto object=*i;
	auto handle=object->handleNativeGet();
	gtk_container_remove(GTK_CONTAINER(widget),handle);
	m_widgets.erase(i);
	}
