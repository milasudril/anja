//@	{"targets":[{"name":"xyplot.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "xyplot.hpp"
#include "container.hpp"
#include <gtk/gtk.h>
#include <vector>
#include <cstring>

using namespace Anja;

XYPlot::Vtable::Vtable() noexcept
	{memset(this,0,sizeof(*this));}

class XYPlot::Impl:public XYPlot
	{
	public:
		explicit Impl(Container& cnt);
		~Impl();

		int id()
			{return m_id;}

		void callback(const Vtable& vt,void* cb,int id) noexcept
			{
			m_vt=vt;
			r_cb_obj=cb;
			m_id=id;
			}

		void curve(const Point* begin,const Point* end,float hue);
		void domain(const Domain& dom) noexcept
			{m_dom=dom;}

		Domain domain() const noexcept
			{return m_dom;}

		XYPlot& cursorX(const Cursor& c);
		XYPlot& cursorX(const Cursor& c,int index);
		Cursor cursorX(int index) const noexcept;

		XYPlot& cursorY(const Cursor& c);
		XYPlot& cursorY(const Cursor& c,int index);
		Cursor cursorY(int index) const noexcept;

	private:
		static void size_changed(GtkWidget* widget,GtkAllocation* allocation,void* obj);
		static gboolean mouse_move(GtkWidget* object,GdkEventMotion* event,void* obj);
		static gboolean mouse_down(GtkWidget* object,GdkEventButton* event,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
		static gboolean key_down(GtkWidget* widget,GdkEventKey* event,void* obj);
		static gboolean key_up(GtkWidget *widget, GdkEventKey *event,void* obj);
		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);

		int m_id;
		Vtable m_vt;
		void* r_cb_obj;
		Domain m_dom;

		typedef std::vector<Point> Curve;

		std::vector<Curve> m_curves;
		std::vector<Cursor> m_cursors_x;
		std::vector<Cursor> m_cursors_y;
		struct TicMark
			{
			double value;
			float extent_x;
			float extent_y;
			char text[16];
			};
		std::vector<TicMark> m_axis_x;
		std::vector<TicMark> m_axis_y;
		GtkDrawingArea* m_canvas;
	};

XYPlot::XYPlot(Container& cnt)
	{
	m_impl=new Impl(cnt);
	}

XYPlot::~XYPlot()
	{
	delete m_impl;
	}



XYPlot::Impl::Impl(Container& cnt):XYPlot(*this),m_id(0)
	{
	auto widget=gtk_drawing_area_new();
	m_canvas=GTK_DRAWING_AREA(widget);
	gtk_widget_set_can_focus(widget,TRUE);
	gtk_widget_add_events(widget
		,GDK_POINTER_MOTION_MASK
		|GDK_BUTTON_PRESS_MASK
		|GDK_BUTTON_RELEASE_MASK
		|GDK_KEY_PRESS_MASK
		|GDK_KEY_RELEASE_MASK);


	g_signal_connect(m_canvas,"draw",G_CALLBACK(draw),this);
	g_signal_connect(m_canvas,"motion-notify-event",G_CALLBACK(mouse_move),this);
	g_signal_connect(m_canvas,"button-press-event",G_CALLBACK(mouse_down),this);
	g_signal_connect(m_canvas,"button-release-event",G_CALLBACK(mouse_up),this);
	g_signal_connect(m_canvas,"key-press-event",G_CALLBACK(key_down),this);
	g_signal_connect(m_canvas,"key-release-event",G_CALLBACK(key_up),this);
	g_signal_connect(m_canvas, "size-allocate", G_CALLBACK(size_changed),this);

	g_object_ref_sink(widget);
	cnt.add(widget);
	domain({{-1,-1},{1,1}});
	gtk_widget_set_size_request(widget,512,128);
	}

XYPlot::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_canvas));
	}


void XYPlot::Impl::size_changed(GtkWidget* widget,GtkAllocation* allocation,void* obj)
	{}

gboolean XYPlot::Impl::mouse_move(GtkWidget* object,GdkEventMotion* event,void* obj)
	{return TRUE;}

gboolean XYPlot::Impl::mouse_down(GtkWidget* object,GdkEventButton* event,void* obj)
	{return TRUE;}

gboolean XYPlot::Impl::mouse_up(GtkWidget* object,GdkEventButton* event,void* obj)
	{return TRUE;}

gboolean XYPlot::Impl::key_down(GtkWidget* widget,GdkEventKey* event,void* obj)
	{return TRUE;}

gboolean XYPlot::Impl::key_up(GtkWidget *widget, GdkEventKey *event,void* obj)
	{return TRUE;}

gboolean XYPlot::Impl::draw(GtkWidget* object,cairo_t* cr,void* obj)
	{return TRUE;}
