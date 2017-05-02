//@	{"targets":[{"name":"xyplot.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "xyplot.hpp"
#include "container.hpp"
#include "../common/color.hpp"
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
		
		size_t m_N_tics_x;
		double m_dx;

		size_t m_N_tics_y;
		double m_dy;
		
		GtkDrawingArea* m_canvas;

		void prerender_x(cairo_t* cr,double x_0,size_t N,double dx);
		void prerender_y(cairo_t* cr,double y_0,size_t N,double dy);
		static Domain window_domain_adjust(cairo_t* cr,int width,int height
			,const std::vector<TicMark>& axis_x
			,const std::vector<TicMark>& axis_y);
		void draw_axis_x(cairo_t* cr,const Domain& dom_window) const;
		void draw_axis_y(cairo_t* cr,const Domain& dom_window) const;

		Point to_window_coords(const Point& p,const Domain& dom_window) const
			{return map_to_domain_inv_y(p,m_dom,dom_window);}

		static Point map_to_domain_inv_y(const Point& point
			,const Domain& source,const Domain& target)
			{
			auto d=source;
			auto w=target;
			auto xi=(point.x-d.min.x)/(d.max.x - d.min.x);
			auto eta=(point.y-d.min.y)/(d.max.y - d.min.y);
			return
				{
				 xi*w.max.x+(1-xi)*w.min.x
				,eta*w.min.y+(1-eta)*w.max.y
				};
			}
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
	m_N_tics_x=10;
	m_dx=0.2;
	m_N_tics_y=10;
	m_dy=0.2;
	gtk_widget_set_size_request(widget,512,128);
	gtk_widget_set_margin_end(widget,4);
	gtk_widget_set_margin_start(widget,4);
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


void XYPlot::Impl::prerender_x(cairo_t* cr,double x_0,size_t N,double dx)
	{
	m_axis_x.clear();
	Point max_extent{0,0};
	for(size_t k=0;k<=N;++k)
		{
		auto pos=x_0+dx*k;
		TicMark tm;
		tm.value=pos;
		sprintf(tm.text,"%.7g",pos);
		cairo_text_extents_t extents_in;
		cairo_text_extents(cr,tm.text,&extents_in);
		tm.extent_x=extents_in.width;
		tm.extent_y=extents_in.height;

		m_axis_x.push_back(tm);
		max_extent.x=tm.extent_x>max_extent.x?
			tm.extent_x : max_extent.x;
		max_extent.y=tm.extent_y>max_extent.y?
			tm.extent_y : max_extent.y;
		}
	m_axis_x.push_back({0,float(max_extent.x),float(max_extent.y)});
	}

void XYPlot::Impl::prerender_y(cairo_t* cr,double y_0,size_t N,double dy)
	{
	m_axis_y.clear();
	float max_extent=0;
	for(size_t k=0;k<=N;++k)
		{
		auto pos=y_0 + dy*k;
		TicMark tm;
		tm.value=fabs(pos)<1e-16?0:pos;
		sprintf(tm.text,"%.7g",tm.value);
		cairo_text_extents_t extents_in;
		cairo_text_extents(cr,tm.text,&extents_in);
		tm.extent_x=extents_in.width;
		tm.extent_y=extents_in.height;

		m_axis_y.push_back(tm);
		max_extent=tm.extent_x>max_extent?
			tm.extent_x : max_extent;
		}
	m_axis_y.push_back({0,max_extent,0});
	}

XYPlot::Domain XYPlot::Impl::window_domain_adjust(cairo_t* cr,int width,int height
	,const std::vector<TicMark>& axis_x
	,const std::vector<TicMark>& axis_y)
	{
	auto margin_top=axis_y.front().extent_y;
	auto margin_bottom=(axis_y.end()-2)->extent_y+1.25*axis_x.back().extent_y;
	auto margin_right=0.5*(axis_x.end()-2)->extent_x;
	auto margin_left=axis_y.back().extent_x+2.5;

	return Domain
		{
			{
			 std::max(margin_left,margin_left+0.5*axis_x.front().extent_x)
			,margin_top
			}
			,{
			 width-margin_right
			,height-margin_bottom
			}
		};
	}


void XYPlot::Impl::draw_axis_y(cairo_t* cr,const Domain& dom_window) const
	{
	auto max_extent=m_axis_y.back().extent_x;
	auto ptr=m_axis_y.data();
	size_t N=m_axis_y.size()-1;
	auto ptr_end=ptr+N;
	size_t k=0;
	while(ptr!=ptr_end)
		{
		auto pos=ptr->value;
		auto point_out=to_window_coords({0,pos},dom_window);
		cairo_move_to(cr,max_extent - ptr->extent_x
			,point_out.y + 0.5*ptr->extent_y);
		cairo_show_text(cr,ptr->text);
		++ptr;
		++k;
		}
	}

void XYPlot::Impl::draw_axis_x(cairo_t* cr,const Domain& dom_window) const
	{
	auto max_extent=m_axis_x.back().extent_y;
	auto ptr=m_axis_x.data();
	size_t N=m_axis_x.size();
	auto ptr_end=ptr+N-1;
	auto domain=m_dom;
	size_t k=0;
	while(ptr!=ptr_end)
		{
		auto pos=ptr->value;
		auto point_out=to_window_coords({pos,domain.min.y},dom_window);
		cairo_move_to(cr,point_out.x-0.5*ptr->extent_x
			,point_out.y+1.25*max_extent);
		cairo_show_text(cr,ptr->text);
		++ptr;
		++k;
		}
	}

gboolean XYPlot::Impl::draw(GtkWidget* widget,cairo_t* cr,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);

	auto w=gtk_widget_get_allocated_width(widget);
	auto h=gtk_widget_get_allocated_height(widget);
	self->prerender_x(cr,self->m_dom.min.x,self->m_N_tics_x,self->m_dx);
	self->prerender_y(cr,self->m_dom.min.y,self->m_N_tics_y,self->m_dy);
	auto dom_window=window_domain_adjust(cr,w,h,self->m_axis_x,self->m_axis_y);


	int dark;
	g_object_get(gtk_settings_get_default(),"gtk-application-prefer-dark-theme"
		,&dark,NULL);
	auto bg=dark?ColorRGBA{0,0,0,1}:ColorRGBA{1,1,1,1};
	auto fg=dark?ColorRGBA{1,1,1,1}:ColorRGBA{0,0,0,1};

	//	Draw plot area background rectangle
		{
		auto w=dom_window.max.x - dom_window.min.x;
		auto h=dom_window.max.y - dom_window.min.y;
		cairo_rectangle(cr,dom_window.min.x,dom_window.min.y,w,h);
		cairo_set_source_rgba(cr,bg.red,bg.green,bg.blue,bg.alpha);
		cairo_fill(cr);
		}

	//	Draw border
		{
		auto w=dom_window.max.x - dom_window.min.x;
		auto h=dom_window.max.y - dom_window.min.y;
		cairo_rectangle(cr,dom_window.min.x,dom_window.min.y,w,h);
		cairo_set_source_rgba(cr,fg.red,fg.green,fg.blue,fg.alpha);
		cairo_set_line_width(cr,1);
		cairo_stroke(cr);
		}

	//	Draw axes
		{
		self->draw_axis_y(cr,dom_window);
		self->draw_axis_x(cr,dom_window);
		}

	return TRUE;
	}
