//@	{"targets":[{"name":"xyplot.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "xyplot.hpp"
#include "container.hpp"
#include "../common/color.hpp"
#include <gtk/gtk.h>
#include <vector>
#include <cstring>
#include <cassert>

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

		int id() const noexcept
			{return m_id;}

		void curve(const Point* begin,const Point* end,float hue);

		void curvesRemove()
			{
			m_curves.clear();
			gtk_widget_queue_draw(GTK_WIDGET(m_canvas));
			}

		Domain domain() const noexcept
			{return m_dom;}

		void domain(const Domain& dom)
			{
			m_dom=dom;
			ticks_count();
			gtk_widget_queue_draw(GTK_WIDGET(m_canvas));
			}

		void cursorX(const Cursor& c)
			{
			m_cursors_x.push_back(c);
			gtk_widget_queue_draw(GTK_WIDGET(m_canvas));
			}

		void cursorX(const Cursor& c,int index)
			{
			if(static_cast<size_t>(index)>=m_cursors_x.size())
				{m_cursors_x.resize(index+1);}
			m_cursors_x[index]=c;
			gtk_widget_queue_draw(GTK_WIDGET(m_canvas));
			}

		Cursor cursorX(int index) const noexcept
			{return m_cursors_x[index];}

		void cursorY(const Cursor& c)
			{
			m_cursors_y.push_back(c);
			gtk_widget_queue_draw(GTK_WIDGET(m_canvas));
			}

		void cursorY(const Cursor& c,int index)
			{
			if(static_cast<size_t>(index)>=m_cursors_y.size())
				{m_cursors_y.resize(index+1);}
			m_cursors_y[index]=c;
			gtk_widget_queue_draw(GTK_WIDGET(m_canvas));
			}

		Cursor cursorY(int index) const noexcept
			{return m_cursors_y[index];}

		void showAll() noexcept
			{domain(m_curves.size()==0?Domain{{-1,-1},{1,1}}:m_dom_full);}




	private:
		static void size_changed(GtkWidget* widget,GtkAllocation* allocation,void* obj);
		static gboolean mouse_move(GtkWidget* object,GdkEventMotion* event,void* obj);
		static gboolean mouse_down(GtkWidget* object,GdkEventButton* event,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
		static gboolean mousewheel(GtkWidget* widget,GdkEvent* event,void* obj);
		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);

		int m_id;
		Vtable m_vt;
		void* r_cb_obj;
		Domain m_dom;

		struct Curve
			{
			float hue;
			std::vector<Point> points;
			};

		std::vector<Curve> m_curves;
		Domain m_dom_full;
		std::vector<Cursor> m_cursors_x;
		std::vector<Cursor> m_cursors_y;

		struct TickMark
			{
			double value;
			float extent_x;
			float extent_y;
			char text[16];
			};
		std::vector<TickMark> m_axis_x;
		std::vector<TickMark> m_axis_y;
		
		size_t m_N_ticks_x;
		double m_dx;

		size_t m_N_ticks_y;
		double m_dy;
		
		GtkDrawingArea* m_canvas;
		bool m_dark;

		enum{NORMAL,PAN,CURSOR_X_MOVE,CURSOR_Y_MOVE,CURSOR_INDEX_MAX};
		GdkCursor* m_cursors[CURSOR_INDEX_MAX];
		int m_cursor_current;
		int m_cursor_grabbed;
		bool m_grabbed;

		void ticks_count();
		void prerender_x(cairo_t* cr,double x_0,size_t N,double dx);
		void prerender_y(cairo_t* cr,double y_0,size_t N,double dy);
		static Domain window_domain_adjust(int width,int height
			,const std::vector<TickMark>& axis_x
			,const std::vector<TickMark>& axis_y);
		void draw_axis_x(cairo_t* cr,const Domain& dom_window) const;
		void draw_axis_y(cairo_t* cr,const Domain& dom_window) const;
		void draw_curve(cairo_t* cr,const Curve& c,const Domain& dom_window,int dark) const; 
		void draw_cursor_x(cairo_t* cr,const Cursor& c,const Domain& dom_window,int dark) const; 
		void draw_cursor_y(cairo_t* cr,const Cursor& c,const Domain& dom_window,int dark) const; 

		Point to_window_coords(const Point& p,const Domain& dom_window) const
			{return map_to_domain_inv_y(p,m_dom,dom_window);}

		Point to_plot_coords(const Point& p,const Domain& dom_window) const
			{return map_to_domain_inv_y(p,dom_window,m_dom);}

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

		static bool inside(const Point& p,const Domain& dom)
			{
			return (p.x>=dom.min.x && p.x<=dom.max.x)
				&& (p.y>=dom.min.y && p.y<=dom.max.y);
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

XYPlot& XYPlot::curve(const Point* begin,const Point* end,float hue)
	{
	m_impl->curve(begin,end,hue);
	return *this;
	}

XYPlot& XYPlot::showAll() noexcept
	{
	m_impl->showAll();
	return *this;
	}

XYPlot& XYPlot::cursorX(const Cursor& c)
	{
	m_impl->cursorX(c);
	return *this;
	}

XYPlot& XYPlot::cursorX(const Cursor& c,int index)
	{
	m_impl->cursorX(c,index);
	return *this;
	}

XYPlot::Cursor XYPlot::cursorX(int index) const noexcept
	{return m_impl->cursorX(index);}

XYPlot& XYPlot::cursorY(const Cursor& c)
	{
	m_impl->cursorY(c);
	return *this;
	}

XYPlot& XYPlot::cursorY(const Cursor& c,int index)
	{
	m_impl->cursorY(c,index);
	return *this;
	}

XYPlot::Cursor XYPlot::cursorY(int index) const noexcept
	{return m_impl->cursorY(index);}

XYPlot& XYPlot::curvesRemove()
	{
	m_impl->curvesRemove();
	return *this;
	}

int XYPlot::id() const noexcept
	{return m_impl->id();}

XYPlot& XYPlot::callback(const Vtable& vt,void* cb_obj,int id) noexcept
	{
	m_impl->callback(vt,cb_obj,id);
	return *this;
	}





XYPlot::Impl::Impl(Container& cnt):XYPlot(*this),m_id(0),r_cb_obj(nullptr)
	{
	auto widget=gtk_drawing_area_new();
	m_canvas=GTK_DRAWING_AREA(widget);
	gtk_widget_set_can_focus(widget,TRUE);
	gtk_widget_add_events(widget
		,GDK_POINTER_MOTION_MASK
		|GDK_BUTTON_PRESS_MASK
		|GDK_BUTTON_RELEASE_MASK
		|GDK_KEY_PRESS_MASK
		|GDK_KEY_RELEASE_MASK
		|GDK_SCROLL_MASK);

	auto display=gdk_display_get_default();
	m_cursors[NORMAL]=gdk_cursor_new_from_name(display,"default");
	m_cursors[PAN]=gdk_cursor_new_from_name(display,"default"); //Move when implemented
	m_cursors[CURSOR_X_MOVE]=gdk_cursor_new_from_name(display,"col-resize");
	m_cursors[CURSOR_Y_MOVE]=gdk_cursor_new_from_name(display,"row-resize");

	g_signal_connect(m_canvas,"draw",G_CALLBACK(draw),this);
	g_signal_connect(m_canvas,"motion-notify-event",G_CALLBACK(mouse_move),this);
	g_signal_connect(m_canvas,"button-press-event",G_CALLBACK(mouse_down),this);
	g_signal_connect(m_canvas,"button-release-event",G_CALLBACK(mouse_up),this);
	g_signal_connect(m_canvas,"scroll-event", G_CALLBACK(mousewheel),this);
	g_signal_connect(m_canvas,"size-allocate", G_CALLBACK(size_changed),this);

	gtk_widget_set_margin_end(widget,4);
	gtk_widget_set_margin_start(widget,4);

	g_object_ref_sink(widget);
	cnt.add(widget);
	domain({{-1,-1},{1,1}});
	m_N_ticks_x=10;
	m_dx=0.2;
	m_N_ticks_y=10;
	m_dy=0.2;
	m_dom_full={{INFINITY,INFINITY},{-INFINITY,-INFINITY}};
	m_dark=0;
	m_grabbed=0;
	m_cursor_grabbed=-1;
	m_cursor_current=NORMAL;
	}

XYPlot::Impl::~Impl()
	{
	m_impl=nullptr;
	for(int k=0;k<CURSOR_INDEX_MAX;++k)
		{g_object_unref(m_cursors[k]);}
	gtk_widget_destroy(GTK_WIDGET(m_canvas));
	}

void XYPlot::Impl::curve(const Point* begin,const Point* end,float hue)
	{
	Curve c;
	c.hue=hue;
	Domain dom=m_dom_full;
	std::for_each(begin,end,[&c,&dom](const Point& p)
		{
		auto M=Point{std::max(dom.max.x,p.x),std::max(dom.max.y,p.y)};
		auto m=Point{std::min(dom.min.x,p.x),std::min(dom.min.y,p.y)};
		dom=Domain{m,M};
		c.points.push_back(p);
		});
	m_curves.push_back(std::move(c));
	m_dom_full=dom;
	gtk_widget_queue_draw(GTK_WIDGET(m_canvas));
	}



void XYPlot::Impl::size_changed(GtkWidget* widget,GtkAllocation* allocation,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	self->ticks_count();
	}

gboolean XYPlot::Impl::mouse_move(GtkWidget* widget,GdkEventMotion* event,void* obj)
	{
	auto w=gtk_widget_get_allocated_width(widget);
	auto h=gtk_widget_get_allocated_height(widget);
	auto self=reinterpret_cast<Impl*>(obj);
	auto dom_window=window_domain_adjust(w,h,self->m_axis_x,self->m_axis_y);
	auto pos_cursor=Point{event->x,event->y};
	if(self->m_grabbed)
		{
		auto pos_cursor_plot=self->to_plot_coords(pos_cursor,dom_window);
		switch(self->m_cursor_current)
			{
			case CURSOR_X_MOVE:
				self->m_cursors_x[self->m_cursor_grabbed].position=pos_cursor_plot.x;
				gtk_widget_queue_draw(widget);
				if(self->r_cb_obj!=nullptr)
					{
					self->m_vt.cursor_x(self->r_cb_obj,*self,self->m_cursor_grabbed
						,keymaskFromSystem(event->state));
					}
				break;
			case CURSOR_Y_MOVE:
				self->m_cursors_y[self->m_cursor_grabbed].position=pos_cursor_plot.y;
				gtk_widget_queue_draw(widget);
				if(self->r_cb_obj!=nullptr)
					{
					self->m_vt.cursor_y(self->r_cb_obj,*self,self->m_cursor_grabbed
						,keymaskFromSystem(event->state));
					}
				break;
			case PAN:
			//	TODO: Requires that cursor offset is saved in mouse_down
				break;
			}
		return TRUE;
		}

	auto parent=gtk_widget_get_parent_window(widget);

	if(!inside(pos_cursor,dom_window))
		{
		if(self->m_cursor_current!=NORMAL && !self->m_grabbed)
			{
			gdk_window_set_cursor(parent,self->m_cursors[NORMAL]);
			self->m_cursor_current=NORMAL;
			}
		return FALSE;
		}
	
	//	Test X cursors
		{
		auto cursor=std::find_if(self->m_cursors_x.begin(),self->m_cursors_x.end()
			,[&dom_window,self,pos_cursor](Cursor& c)
				{
				auto position=self->to_window_coords(Point{c.position,0},dom_window);
				return std::abs(pos_cursor.x - position.x)<2;
				});
		if(cursor!=self->m_cursors_x.end())
			{
			self->m_cursor_grabbed=cursor - self->m_cursors_x.begin();
			gdk_window_set_cursor(parent,self->m_cursors[CURSOR_X_MOVE]);
			self->m_cursor_current=CURSOR_X_MOVE;
			return TRUE;
			}
		}

	//	Test Y cursors
		{
		auto cursor=std::find_if(self->m_cursors_y.begin(),self->m_cursors_y.end()
			,[&dom_window,self,pos_cursor](Cursor& c)
				{
				auto position=self->to_window_coords(Point{0,c.position},dom_window);
				return std::abs(pos_cursor.y - position.y)<2;
				});
		if(cursor!=self->m_cursors_y.end())
			{
			self->m_cursor_grabbed=cursor - self->m_cursors_y.begin();
			gdk_window_set_cursor(parent,self->m_cursors[CURSOR_Y_MOVE]);
			self->m_cursor_current=CURSOR_Y_MOVE;
			return TRUE;
			}
		}

	gdk_window_set_cursor(parent,self->m_cursors[PAN]);
	self->m_cursor_current=PAN;
	return TRUE;
	}

gboolean XYPlot::Impl::mouse_down(GtkWidget* widget,GdkEventButton* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	if(event->button==1 && self->m_cursor_current!=NORMAL)
		{self->m_grabbed=1;}

	return TRUE;
	}

gboolean XYPlot::Impl::mouse_up(GtkWidget* object,GdkEventButton* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	if(event->button==3)
		{self->showAll();}
	if(event->button==1)
		{self->m_grabbed=0;}
	return TRUE;
	}

gboolean XYPlot::Impl::mousewheel(GtkWidget* widget,GdkEvent* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	auto w=gtk_widget_get_allocated_width(widget);
	auto h=gtk_widget_get_allocated_height(widget);
	auto dom_window=window_domain_adjust(w,h,self->m_axis_x,self->m_axis_y);
	auto& e=event->scroll;

	if(!inside(Point{e.x,e.y},dom_window))
		{return FALSE;}

	auto factor_x=1.0;
	auto factor_y=1.0;
	if(e.state&GDK_CONTROL_MASK)
		{factor_x=e.direction==GDK_SCROLL_UP?0.8:1.25;}
	if(e.state&GDK_SHIFT_MASK)
		{factor_y=e.direction==GDK_SCROLL_UP?0.8:1.25;}

	auto dom=self->m_dom;
	auto pos=self->to_plot_coords(Point{e.x,e.y},dom_window);

	if(e.state&GDK_CONTROL_MASK)
		{
		auto w_in=dom.max.x - dom.min.x;
		auto w=factor_x*w_in;
		if(w>2*(self->m_dom_full.max.x - self->m_dom_full.min.x) 
			|| w<1e-7*(self->m_dom_full.max.x - self->m_dom_full.min.x))
			{return TRUE;}
		auto x=w*(pos.x - dom.min.x)/w_in;
		dom.min.x=pos.x - x;
		dom.max.x=dom.min.x + w;		
		}
	
	if(e.state&GDK_SHIFT_MASK)
		{
		auto h_in=dom.max.y - dom.min.y;
		auto h=factor_y*h_in;
		if(h>2*(self->m_dom_full.max.y - self->m_dom_full.min.y) 
			|| h<1e-7*(self->m_dom_full.max.y - self->m_dom_full.min.y))
			{return TRUE;}
		auto y=h*(pos.y - dom.min.y)/h_in;
		dom.min.y=pos.y - y;
		dom.max.y=dom.min.y + h;	
		}

	self->domain(dom);
	return TRUE;
	}



static double nicenum(double x)
	{
    auto exponent = std::floor(log10(x));
    auto fraction = x/std::pow(10.0,exponent);
	double nicefrac=[](double fraction)
		{
		if(fraction<1.5)
			{return 1.0;}
		if(fraction<3.0)
			{return 2.0;}
		if(fraction<7.0)
			{return 5.0;}
		return 10.0;
		}(fraction);
    return nicefrac*std::pow(10.0,exponent);
	}


void XYPlot::Impl::ticks_count()
	{
	auto domain=m_dom;
	auto cr=gdk_cairo_create(gtk_widget_get_window(GTK_WIDGET(m_canvas)));
	if(cr==NULL)
		{return;}

	auto w_window=gtk_widget_get_allocated_width(GTK_WIDGET(m_canvas));
	auto h_window=gtk_widget_get_allocated_height(GTK_WIDGET(m_canvas));

	auto w=domain.max.x - domain.min.x;
	auto h=domain.max.y - domain.min.y;

//	Render axes with with two ticks. This is required in order to compute the margin
//	needed for axes
	auto N_y=2;
	auto dy=h/N_y;
	prerender_y(cr,domain.min.y,N_y,dy);

	auto N_x=2;
	auto dx=w/N_x;
	prerender_x(cr,domain.min.x,N_x,dx);

	auto dom_window=window_domain_adjust(w_window,h_window,m_axis_x,m_axis_y);

//	Compute Y ticks
		{
		auto h_dom=dom_window.max.y - dom_window.min.y;
		auto N_y_temp=h_dom/(4*m_axis_y.front().extent_y); //Make each tick 4 units high
		if(N_y_temp==0.0)
			{
		//Skip it (height was zero)
			cairo_destroy(cr);
			return;
			}
		dy=nicenum( h/N_y_temp ); //Update dy using the number of ticks
		m_N_ticks_y=h/dy; //Save values
		m_dy=dy;
		}

	// Compute X ticks
		{
		auto fits=[cr,w,&domain,this,w_window,h_window](int N_x)
			{
			auto dx=nicenum(w/N_x);
			prerender_x(cr,dx*std::ceil(domain.min.x/dx),N_x,dx);
			auto dx_min=m_axis_x.back().extent_x+16; //The largest possible extent for any label
		//	Compute the number of pixels for dx. This value must be larger than dx_min
			auto dom_window=window_domain_adjust(w_window,h_window,m_axis_x,m_axis_y);
			auto dom_win_width=dom_window.max.x - dom_window.min.x;
			auto dx_win=dx * dom_win_width/w;
			return dx_win > dx_min;
			};

	//	Find an upper bound
		auto N_x_upper=N_x;
		while( fits(N_x_upper) )
			{N_x_upper<<=1;}
	//	Do binary search
		auto N_x_lower=2;
		auto mid=0;
		while(N_x_lower < N_x_upper)
			{	
			mid=(N_x_upper + N_x_lower)/2; //There shoule be no risk for overflow
			if(fits(mid))
				{N_x_lower=mid + 1;}
			else
				{N_x_upper=mid;}
			}
		dx=nicenum(w/(mid-1));
		m_N_ticks_x=w/dx;
		m_dx=dx;
		};


	cairo_destroy(cr);
	}

void XYPlot::Impl::prerender_x(cairo_t* cr,double x_0,size_t N,double dx)
	{
	m_axis_x.clear();
	Point max_extent{0,0};
	for(size_t k=0;k<=N;++k)
		{
		auto pos=x_0+dx*k;
		TickMark tm;
		tm.value=fabs(pos)<1e-16?0:pos;
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
		TickMark tm;
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
	m_axis_y.push_back({0,float(max_extent),0});
	}

XYPlot::Domain XYPlot::Impl::window_domain_adjust(int width,int height
	,const std::vector<TickMark>& axis_x
	,const std::vector<TickMark>& axis_y)
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
		if(pos>m_dom.max.y)
			{return;}
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
		if(pos>m_dom.max.x)
			{return;}
		auto point_out=to_window_coords({pos,domain.min.y},dom_window);
		cairo_move_to(cr,point_out.x-0.5*ptr->extent_x
			,point_out.y+1.25*max_extent+4);
		cairo_show_text(cr,ptr->text);
		++ptr;
		++k;
		}
	}

void XYPlot::Impl::draw_curve(cairo_t* cr,const Curve& c,const Domain& dom_window
	,int dark) const
	{
	if(c.points.size()==0)
		{return;}

	ColorRGBA color(ColorHSLA::fromHueAndLuma(c.hue,dark?0.7:0.4));
	cairo_set_source_rgba(cr,color.red,color.green,color.blue,color.alpha);

	auto i=c.points.begin();

	while(i!=c.points.end())
		{
		i=std::find_if(i,c.points.end(),[&dom_window,this](const Point& p)
			{
			auto point_out=to_window_coords(p,dom_window);
			return inside(point_out,dom_window);
			});
		if(i==c.points.end())
			{return;}

	//	TODO: draw line segment to the edge of the box
		auto point_out=to_window_coords(*i,dom_window);
		cairo_move_to(cr,point_out.x,point_out.y);
		i=std::find_if(i,c.points.end()
			,[cr,this,&dom_window,&point_out](const Point& p)
			{
			auto point_out_next=to_window_coords(p,dom_window);
			if(!inside(point_out_next,dom_window))
				{
			//	TODO: draw line segment to the edge of the box
				return 1;
				}
			auto dx=point_out_next.x - point_out.x;
			auto dy=point_out_next.y - point_out.y;
			if(dx*dx + dy*dy > 4)
				{
				cairo_line_to(cr,point_out.x,point_out.y);
				point_out=point_out_next;
				}
			return 0;
			});
		cairo_stroke(cr);
		}
	}

void XYPlot::Impl::draw_cursor_x(cairo_t* cr,const Cursor& c,const Domain& dom_window,int dark) const
	{
	auto point_out=to_window_coords(Point{c.position,m_dom.min.y},dom_window);
	if(point_out.x<dom_window.min.x || point_out.x>dom_window.max.x)
		{return;}
	ColorRGBA color(ColorHSLA::fromHueAndLuma(c.hue,dark?0.7:0.4));
	cairo_set_source_rgba(cr,color.red,color.green,color.blue,color.alpha);
	cairo_move_to(cr,point_out.x,point_out.y);
	point_out=to_window_coords(Point{c.position,m_dom.max.y},dom_window);
	cairo_line_to(cr,point_out.x,point_out.y);
	cairo_stroke(cr);
	}

void XYPlot::Impl::draw_cursor_y(cairo_t* cr,const Cursor& c,const Domain& dom_window,int dark) const
	{
	auto point_out=to_window_coords(Point{m_dom.min.x,c.position},dom_window);
	if(point_out.y<dom_window.min.y || point_out.y>dom_window.max.y)
		{return;}
	ColorRGBA color(ColorHSLA::fromHueAndLuma(c.hue,dark?0.7:0.4));
	cairo_set_source_rgba(cr,color.red,color.green,color.blue,color.alpha);
	cairo_move_to(cr,point_out.x,point_out.y);
	point_out=to_window_coords(Point{m_dom.max.x,c.position},dom_window);
	cairo_line_to(cr,point_out.x,point_out.y);
	cairo_stroke(cr);
	}

static int dark_check(GtkWidget* widget)
	{
	auto context=gtk_widget_get_style_context(widget);
	GdkRGBA color;
	gtk_style_context_get_color(context,GTK_STATE_FLAG_NORMAL,&color);
	return luma709(ColorRGBA(color.red,color.green,color.blue,color.alpha)) > 0.5f;
	}

gboolean XYPlot::Impl::draw(GtkWidget* widget,cairo_t* cr,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);

	self->prerender_x(cr,self->m_dx*std::ceil(self->m_dom.min.x/self->m_dx)
		,self->m_N_ticks_x,self->m_dx);
	self->prerender_y(cr,self->m_dy*std::ceil(self->m_dom.min.y/self->m_dy)
		,self->m_N_ticks_y,self->m_dy);
	auto w=gtk_widget_get_allocated_width(GTK_WIDGET(self->m_canvas));
	auto h=gtk_widget_get_allocated_height(GTK_WIDGET(self->m_canvas));
	auto dom_window=window_domain_adjust(w,h,self->m_axis_x,self->m_axis_y);

//	dark_check is only reliable for sensitive widget in active window
	auto state=gtk_widget_get_state_flags(widget);
	self->m_dark=state&(GTK_STATE_FLAG_BACKDROP|GTK_STATE_FLAG_INSENSITIVE)?
		self->m_dark:dark_check(widget);

	auto bg=self->m_dark?ColorRGBA{0,0,0,1}:ColorRGBA{1,1,1,1};
	auto fg=self->m_dark?ColorRGBA{1,1,1,1}:ColorRGBA{0,0,0,1};
	if(state & (GTK_STATE_FLAG_BACKDROP|GTK_STATE_FLAG_INSENSITIVE))
		{
		auto bg_temp=bg;
		bg.red=0.9*bg.red + 0.1*fg.red;
		bg.green=0.9*bg.green + 0.1*fg.green;
		bg.blue=0.9*bg.blue + 0.1*fg.blue;

		fg.red=0.6*fg.red + 0.4*bg_temp.red;
		fg.green=0.6*fg.green + 0.4*bg_temp.green;
		fg.blue=0.6*fg.blue + 0.4*bg_temp.blue;
		}

	//	Draw plot area background rectangle
		{
		auto w=dom_window.max.x - dom_window.min.x;
		auto h=dom_window.max.y - dom_window.min.y;
		cairo_rectangle(cr,dom_window.min.x,dom_window.min.y,w,h);
		cairo_set_source_rgba(cr,bg.red,bg.green,bg.blue,bg.alpha);
		cairo_fill(cr);
		}

	//	Draw curves
		cairo_set_line_width(cr,1);
		std::for_each(self->m_curves.begin(),self->m_curves.end()
			,[&cr,&dom_window,self](const Curve& c)
				{self->draw_curve(cr,c,dom_window,self->m_dark);});

	//	Draw cursors
		std::for_each(self->m_cursors_x.begin(),self->m_cursors_x.end()
			,[&cr,&dom_window,self](const Cursor& c)
				{self->draw_cursor_x(cr,c,dom_window,self->m_dark);});

		std::for_each(self->m_cursors_y.begin(),self->m_cursors_y.end()
			,[&cr,&dom_window,self](const Cursor& c)
				{self->draw_cursor_y(cr,c,dom_window,self->m_dark);});



	//	Draw border
		{
		auto w=dom_window.max.x - dom_window.min.x;
		auto h=dom_window.max.y - dom_window.min.y;
		cairo_rectangle(cr,dom_window.min.x-1,dom_window.min.y-1,w+2,h+2);
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
