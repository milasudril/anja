#ifdef __WAND__
target
	[
	name[boxhorizontal.o]
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

#include "boxhorizontal.h"
#include "guihandle.h"
#include "arraydynamicshort.h"
#include <algorithm>

class BoxHorizontalGtk:public BoxHorizontal
	{
	public:
		BoxHorizontalGtk(GuiContainer& parent,EventHandler* event_handler);
		~BoxHorizontalGtk();

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_box;}

		void insertModeSet(uint32_t mode)
			{m_insert_mode=mode;}

		void componentAdd(Widget& widget);
		void componentRemove(Widget& widget);

		void slaveAssign(Widget& widget)
			{r_slave=&widget;}

		void slaveRelease()
			{r_slave=nullptr;}

	private:
		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_box;
		Widget* r_slave;
		uint32_t m_insert_mode;

		ArrayDynamicShort<Widget*> m_widgets;
		void componentRemoveAt(const ArrayDynamicShort<Widget*>::iterator& i);
	};

BoxHorizontal* BoxHorizontal::create(GuiContainer& parent,EventHandler* handler)
	{return new BoxHorizontalGtk(parent,handler);}

void BoxHorizontalGtk::destroy()
	{delete this;}

BoxHorizontalGtk::BoxHorizontalGtk(GuiContainer& parent,EventHandler* handler):
	r_parent(parent),r_handler(handler),r_slave(nullptr),m_insert_mode(0)
	{
	GtkWidget* box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
	m_box=box;
	g_object_ref_sink(box);
	r_parent.componentAdd(*this);
	gtk_widget_show(box);
	}

BoxHorizontalGtk::~BoxHorizontalGtk()
	{
	if(r_slave!=nullptr)
		{r_slave->destroy();}

	std::for_each(m_widgets.rbegin(),m_widgets.rend(),widgetDestroy);
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_box);
	}

void BoxHorizontalGtk::componentAdd(Widget& widget)
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
	if(insert_mode&INSERTMODE_TOP)
		{gtk_widget_set_valign(h,GTK_ALIGN_START);}
	gtk_widget_show(h);
	}

void BoxHorizontalGtk::componentRemove(Widget& widget)
	{
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,&widget);
	if(i!=m_widgets.end())
		{
		componentRemoveAt(i);
		}
	}

void BoxHorizontalGtk::componentRemoveAt(const ArrayDynamicShort<Widget*>::iterator& i)
	{
	GtkWidget* widget=m_box;
	auto object=*i;
	auto handle=object->handleNativeGet();
	gtk_container_remove(GTK_CONTAINER(widget),handle);
	m_widgets.erase(i);
	}
