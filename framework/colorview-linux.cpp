#ifdef __WAND__
target
	[
	name[colorview.o]
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

#include "colorview.h"
#include "guicontainer.h"
#include "guihandle.h"
#include "exceptionswallow.h"
#include "colorsystem.h"
#include <gtk/gtk.h>

class ColorViewGtk:public ColorView
	{
	public:
		ColorViewGtk(GuiContainer& parent,EventHandler& handler,unsigned int id);

		~ColorViewGtk();

		void destroy()
			{delete this;}


		unsigned int idGet() const
			{return m_id;}

		const ColorRGBA& colorGet() const
			{return m_color;}

		void colorSet(const ColorRGBA& color)
			{
			m_color=color;
			update();
			}

		const GuiHandle& handleNativeGet() const
			{return m_rect;}

	private:
		void update()
			{
			GtkWidget* rect=m_rect;
			gtk_widget_queue_draw(rect);
			}

		GuiContainer& r_parent;
		EventHandler& r_handler;
		GuiHandle m_rect;
		ColorRGBA m_color;
		unsigned int m_id;

		static gboolean onMouseUp(GtkWidget* object,GdkEventButton* event,void* colorviewgtk);
		static gboolean onMouseDown(GtkWidget* object,GdkEventButton* event,void* colorviewgtk);
		static gboolean onDraw(GtkWidget* widget,cairo_t* cr,void* knobgtk);
	};

ColorView::EventHandler ColorView::s_default_handler;

gboolean ColorViewGtk::onMouseUp(GtkWidget* object,GdkEventButton* event,void* colorviewgtk)
	{
	ColorViewGtk* _this=reinterpret_cast<ColorViewGtk*>(colorviewgtk);
	EXCEPTION_SWALLOW(
		 _this->r_handler.onActionPerform(*_this);
		,_this);
	return TRUE;
	}

gboolean ColorViewGtk::onMouseDown(GtkWidget* object,GdkEventButton* event,void* colorviewgtk)
	{
	gtk_widget_grab_focus(object);
	return TRUE;
	}

gboolean ColorViewGtk::onDraw(GtkWidget* widget,cairo_t* cr,void* colorviewgtk)
	{
	ColorViewGtk* _this=reinterpret_cast<ColorViewGtk*>(colorviewgtk);

	ColorSystem color_gtk=_this->m_color;
	gdk_cairo_set_source_rgba(cr,color_gtk);
	auto width=gtk_widget_get_allocated_width (widget);
	auto height=gtk_widget_get_allocated_height(widget);

	cairo_rectangle(cr,0,0,width,height);
	cairo_fill_preserve(cr);

	color_gtk=COLORS[ColorID::BLACK];
	gdk_cairo_set_source_rgba(cr,color_gtk);
	cairo_stroke(cr);

	return FALSE;
	}



ColorView* ColorView::create(GuiContainer& parent,EventHandler& handler
	,unsigned int id)
	{return new ColorViewGtk(parent,handler,id);}

ColorViewGtk::ColorViewGtk(GuiContainer& parent,EventHandler& handler
	,unsigned int id):r_parent(parent),r_handler(handler)
	,m_color(COLORS[ColorID::BLACK]),m_id(id)
	{
	GtkWidget* rect=gtk_drawing_area_new();
	gtk_widget_set_size_request(rect,32,24);
	gtk_widget_add_events(rect,GDK_BUTTON_RELEASE_MASK
		|GDK_BUTTON_PRESS_MASK);
	g_signal_connect(rect,"draw",G_CALLBACK(onDraw),this);
	g_signal_connect(rect,"button-release-event",G_CALLBACK(onMouseUp),this);
	g_signal_connect(rect,"button-press-event",G_CALLBACK(onMouseDown),this);
	m_rect=rect;
	g_object_ref_sink(rect);
	parent.componentAdd(*this);
	}

ColorViewGtk::~ColorViewGtk()
	{
	r_parent.componentRemove(*this);
	GtkWidget* rect=m_rect;
	gtk_widget_destroy(rect);
	}

