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
#include "exceptionswallow.h"
#include <algorithm>

class BoxHorizontalGtk:public BoxHorizontal
	{
	public:
		BoxHorizontalGtk(GuiContainer& parent,EventHandler& event_handler);
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
			{
			r_slave=nullptr;
			}

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

	private:
		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_box;
		Widget* r_slave;
		uint32_t m_insert_mode;

		static gboolean onMouseMove(GtkWidget* object,GdkEventMotion* event
			,void* boxhorizontalgtk);

		static gboolean onMouseDown(GtkWidget* object,GdkEventButton* event
			,void* boxhorizontalgtk);

		static gboolean onMouseUp(GtkWidget* object,GdkEventButton* event
			,void* boxhorizontalgtk);

		static gboolean onKeyDown(GtkWidget* widget,GdkEventKey* event
			,void* boxhorizontalgtk);

		static gboolean onKeyUp(GtkWidget* widget,GdkEventKey* event
			,void* boxhorizontalgtk);

		ArrayDynamicShort<Widget*> m_widgets;
		void componentRemoveAt(const ArrayDynamicShort<Widget*>::iterator& i);
	};


gboolean BoxHorizontalGtk::onMouseMove(GtkWidget* object,GdkEventMotion* event
	,void* windowgtk)
	{
	BoxHorizontalGtk* _this=(BoxHorizontalGtk*)windowgtk;
	EXCEPTION_SWALLOW(
		_this->r_handler->onMouseMove(*_this,event->x,event->y,keymaskFromSystem(event->state));
		,_this);
	return TRUE;
	}

gboolean BoxHorizontalGtk::onMouseDown(GtkWidget* widget,GdkEventButton* event
	,void* windowgtk)
	{
	BoxHorizontalGtk* _this=(BoxHorizontalGtk*)windowgtk;
	EXCEPTION_SWALLOW(
		_this->r_handler->onMouseDown(*_this,event->x,event->y,keymaskFromSystem(event->state));
		,_this);
	return TRUE;
	}

gboolean BoxHorizontalGtk::onMouseUp(GtkWidget* widget,GdkEventButton* event
	,void* windowgtk)
	{
	BoxHorizontalGtk* _this=(BoxHorizontalGtk*)windowgtk;
	EXCEPTION_SWALLOW(
		_this->r_handler->onMouseUp(*_this,event->x,event->y,keymaskFromSystem(event->state));
		,_this);
	return TRUE;
	}

gboolean BoxHorizontalGtk::onKeyDown(GtkWidget* widget,GdkEventKey* event
	,void* windowgtk)
	{
	BoxHorizontalGtk* _this=(BoxHorizontalGtk*)windowgtk;
	EXCEPTION_SWALLOW(_this->r_handler->onKeyDown(*_this,keymaskFromSystem(event->state));
		,_this);
	return FALSE;
	}

gboolean BoxHorizontalGtk::onKeyUp(GtkWidget* widget,GdkEventKey* event
	,void* windowgtk)
	{
	BoxHorizontalGtk* _this=(BoxHorizontalGtk*)windowgtk;
	EXCEPTION_SWALLOW(_this->r_handler->onKeyDown(*_this,keymaskFromSystem(event->state));
		,_this);
	return FALSE;
	}

BoxHorizontal::EventHandler BoxHorizontal::s_default_handler;

BoxHorizontal* BoxHorizontal::create(GuiContainer& parent,EventHandler& handler)
	{return new BoxHorizontalGtk(parent,handler);}

void BoxHorizontalGtk::destroy()
	{
	if(r_slave==nullptr)
		{delete this;}
	else
		{r_slave->destroy();}
	}

BoxHorizontalGtk::BoxHorizontalGtk(GuiContainer& parent,EventHandler& handler):
	r_parent(parent),r_handler(&handler),r_slave(nullptr),m_insert_mode(0)
	{
	GtkWidget* box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
	m_box=box;
	g_object_ref_sink(box);
	gtk_widget_set_can_focus(m_box,TRUE);
	g_signal_connect(box,"button-release-event",G_CALLBACK(onMouseUp),this);
	g_signal_connect(box,"button-press-event",G_CALLBACK(onMouseDown),this);
	g_signal_connect(box,"motion-notify-event",G_CALLBACK(onMouseMove),this);
	g_signal_connect(box,"key_press_event",G_CALLBACK(onKeyDown),this);
	g_signal_connect(box,"key_release_event",G_CALLBACK(onKeyUp),this);

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
