#ifdef __WAND__
target
	[
	name[xyplot.o]
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

#include "xyplot.h"
#include "guicontainer.h"
#include "curve.h"
#include "colorsystem.h"
#include "guihandle.h"

#include <gtk/gtk.h>

#include <vector>
#include <cmath>

class XYPlotGtk:public XYPlot
	{
	public:
		XYPlotGtk(GuiContainer& parent,EventHandler& handler);

		void update()
			{gtk_widget_queue_draw(m_canvas);}

		void curveAdd(const Curve& curve);

		void domainSet(const Curve::Domain& domain_new)
			{
			m_domain_current=domain_new;
			ticksUpdate();
			}

		const Curve::Domain& domainGet() const
			{return m_domain_current_active;}

		void backgroundSet(bool light);

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_canvas;}

		void eventHandlerSet(EventHandler& handler)
			{r_handler=&handler;}


		size_t cursorXAdd(const Cursor& cursor)
			{
			m_cursors_x.push_back(cursor);
			update();
			return m_cursors_x.size()-1;
			}

		void cursorXRemove(size_t n)
			{m_cursors_x.erase(m_cursors_y.begin()+n);}

		void cursorXSet(size_t n,const Cursor& cursor)
			{
			m_cursors_x[n]=cursor;
			update();
			}

		const Cursor& cursorXGet(size_t n) const
			{return m_cursors_x[n];}

		Cursor& cursorXGet(size_t n)
			{return m_cursors_x[n];}


		size_t cursorYAdd(const Cursor& cursor)
			{
			m_cursors_y.push_back(cursor);
			update();
			return m_cursors_y.size()-1;
			}

		void cursorYRemove(size_t n)
			{m_cursors_y.erase(m_cursors_y.begin()+n);}

		void cursorYSet(size_t n,const Cursor& cursor)
			{
			m_cursors_y[n]=cursor;
			update();
			}

		const Cursor& cursorYGet(size_t n) const
			{return m_cursors_y[n];}

		Cursor& cursorYGet(size_t n)
			{return m_cursors_y[n];}

		void cursorsRemove()
			{
			m_cursors_x.clear();
			m_cursors_y.clear();
			update();
			}

	private:
		static void onSizeChange(GtkWidget* widget,GtkAllocation* allocation
			,void* xyplotgtk);

		static gboolean onMouseMove(GtkWidget* object,GdkEventMotion* event
			,void* xyplotgtk);

		static gboolean onMouseDown(GtkWidget* object,GdkEventButton* event
			,void* xyplotgtk);

		static gboolean onMouseUp(GtkWidget* object,GdkEventButton* event
			,void* xyplotgtk);

		static gboolean onKeyDown(GtkWidget* widget,GdkEventKey* event
			,void* xyplotgtk);

		static gboolean onKeyUp(GtkWidget *widget, GdkEventKey *event
			,void* xyplotgtk);

		static gboolean onPaint(GtkWidget* object,cairo_t* cr,void* xyplotgtk);


		struct alignas(32) TicMark
			{
			double value;
			float extent_x;
			float extent_y;
			char text[16];
			};

		void axisXExtents(cairo_t* cr,double x_0,size_t N,double delta_x);
		void axisYExtents(cairo_t* cr,double y_0,size_t N,double delta_y);
		void ticksUpdate();

		Curve::Point toWindowCoords(const Curve::Point& p) const
			{
			return mapToDomainInvertY(p,m_domain_current_active,m_domain_window);
			}

		Curve::Point toPlotCoords(const Curve::Point& p) const
			{
			return mapToDomainInvertY(p,m_domain_window,m_domain_current_active);
			}

		XYPlot::Cursor* cursorXAtPoint(const Curve::Point& p, double tol);
		XYPlot::Cursor* cursorYAtPoint(const Curve::Point& p, double tol);

		void windowDomainAdjust(cairo_t* cr,int width,int height);
		void axisYDraw(cairo_t* cr) const;
		void axisXDraw(cairo_t* cr) const;
		void curveDraw(cairo_t* cr,const Curve& curve) const;

		GuiContainer& r_parent;
		EventHandler* r_handler;

		std::vector<Curve> m_curves;
		std::vector<Cursor> m_cursors_x;
		std::vector<Cursor> m_cursors_y;
		std::vector<TicMark> m_axis_x;
		std::vector<TicMark> m_axis_y;
		GuiHandle m_canvas;

		double m_width;
		size_t m_N_max_x;
		double m_delta_x;

		double m_height;
		size_t m_N_max_y;
		double m_delta_y;


		Curve::Domain m_domain_max;
		Curve::Domain m_domain_current;
		Curve::Domain m_domain_current_active;
		Curve::Domain m_domain_window;

		Color m_background_color;
		Color m_text_color;
		Color m_canvas_color;
		float m_curve_luma;
	};

XYPlot::EventHandler XYPlot::s_default_handler;

XYPlot* XYPlot::create(GuiContainer& parent,EventHandler& handler)
	{return new XYPlotGtk(parent,handler);}

void XYPlotGtk::onSizeChange(GtkWidget* widget,GtkAllocation* allocation
	,void* xyplotgtk)
	{
	XYPlotGtk* _this=(XYPlotGtk*)xyplotgtk;
	_this->m_width=allocation->width;
	_this->m_height=allocation->height;

	_this->ticksUpdate();
	_this->update();
	}

gboolean XYPlotGtk::onMouseMove(GtkWidget* object
	,GdkEventMotion* event,void* xyplotgtk)
	{
	XYPlotGtk* _this=(XYPlotGtk*)xyplotgtk;
	auto point=_this->toPlotCoords(Curve::Point{event->x,event->y});
	if(belongsTo(point,_this->m_domain_current_active))
		{_this->r_handler->onMouseMove(*_this,point,keymaskFromSystem(event->state));}
	return TRUE;
	}

gboolean XYPlotGtk::onMouseDown(GtkWidget* object,GdkEventButton* event
	,void* xyplotgtk)
	{
	gtk_widget_grab_focus(object);
	XYPlotGtk* _this=(XYPlotGtk*)xyplotgtk;
	auto point=_this->toPlotCoords(Curve::Point{event->x,event->y});
	if(belongsTo(point,_this->m_domain_current_active))
		{
		auto x_min=mapToDomainX(0,_this->m_domain_window,_this->m_domain_current_active);
		auto x_max=mapToDomainX(4,_this->m_domain_window,_this->m_domain_current_active);
		auto tol_x=x_max-x_min;

		auto y_min=mapToDomainY(0,_this->m_domain_window,_this->m_domain_current_active);
		auto y_max=mapToDomainY(4,_this->m_domain_window,_this->m_domain_current_active);
		auto tol_y=y_max-y_min;

		auto cursor=_this->cursorXAtPoint(point,tol_x);
		if(cursor!=nullptr)
			{
			_this->r_handler->onCursorXHit(*_this,*cursor,keymaskFromSystem(event->state));
			}

		cursor=_this->cursorYAtPoint(point,tol_y);
		if(cursor!=nullptr)
			{
			_this->r_handler->onCursorYHit(*_this,*cursor,keymaskFromSystem(event->state));
			return TRUE;
			}
		else
			{_this->r_handler->onMouseDown(*_this,point,keymaskFromSystem(event->state));}
		}
	return TRUE;
	}

gboolean XYPlotGtk::onMouseUp(GtkWidget* object,GdkEventButton* event
	,void* xyplotgtk)
	{
	XYPlotGtk* _this=(XYPlotGtk*)xyplotgtk;
	auto point=_this->toPlotCoords(Curve::Point{event->x,event->y});
	if(belongsTo(point,_this->m_domain_current_active))
		{_this->r_handler->onMouseUp(*_this,point,keymaskFromSystem(event->state));}
	return TRUE;
	}

gboolean XYPlotGtk::onKeyDown(GtkWidget* widget,GdkEventKey* event
	,void* xyplotgtk)
	{
	XYPlotGtk* _this=(XYPlotGtk*)xyplotgtk;
	_this->r_handler->onKeyDown(*_this,event->hardware_keycode-8);
	return TRUE;
	}

gboolean XYPlotGtk::onKeyUp(GtkWidget *widget, GdkEventKey *event
	,void* xyplotgtk)
	{
	XYPlotGtk* _this=(XYPlotGtk*)xyplotgtk;
	_this->r_handler->onKeyUp(*_this,event->hardware_keycode-8);
	return TRUE;
	}

XYPlot::Cursor* XYPlotGtk::cursorXAtPoint(const Curve::Point& p,double tol)
	{
	auto ptr_cursor=m_cursors_x.data();
	auto ptr_cursors_end=ptr_cursor+m_cursors_x.size();
	while(ptr_cursor!=ptr_cursors_end)
		{
		if(std::abs(ptr_cursor->position - p.x) < tol)
			{return ptr_cursor;}
		++ptr_cursor;
		}
	return nullptr;
	}

XYPlot::Cursor* XYPlotGtk::cursorYAtPoint(const Curve::Point& p,double tol)
	{
	auto ptr_cursor=m_cursors_y.data();
	auto ptr_cursors_end=ptr_cursor+m_cursors_y.size();

	while(ptr_cursor!=ptr_cursors_end)
		{
		if(std::abs(ptr_cursor->position - p.y) < tol)
			{return ptr_cursor;}
		++ptr_cursor;
		}
	return nullptr;
	}

XYPlotGtk::XYPlotGtk(GuiContainer& parent,EventHandler& handler):
	r_parent(parent),r_handler(&handler),m_width{0},m_height{0}
	{
	m_canvas=gtk_drawing_area_new();

	gtk_widget_set_can_focus(m_canvas,TRUE);
		gtk_widget_add_events(m_canvas
		,GDK_POINTER_MOTION_MASK
		|GDK_BUTTON_PRESS_MASK
		|GDK_BUTTON_RELEASE_MASK
		|GDK_KEY_PRESS_MASK
		|GDK_KEY_RELEASE_MASK);

	g_signal_connect(m_canvas,"draw",G_CALLBACK(onPaint),this);
	g_signal_connect(m_canvas,"motion-notify-event",G_CALLBACK(onMouseMove),this);
	g_signal_connect(m_canvas,"button-press-event",G_CALLBACK(onMouseDown),this);
	g_signal_connect(m_canvas,"button-release-event",G_CALLBACK(onMouseUp),this);
	g_signal_connect(m_canvas,"key_press_event",G_CALLBACK(onKeyDown),this);
	g_signal_connect(m_canvas,"key_release_event",G_CALLBACK(onKeyUp),this);
	g_signal_connect(m_canvas, "size-allocate", G_CALLBACK(onSizeChange),this);

	g_object_ref(m_canvas);
	r_parent.componentAdd(*this);
	domainSet({-1,-1,1,1});
	backgroundSet(1);
	}

void XYPlotGtk::destroy()
	{
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_canvas);
	delete this;
	}

void XYPlotGtk::curveAdd(const Curve& curve)
	{
	m_curves.push_back(curve);
	auto d=curve.domainGet();
	Curve::Domain domain_new=m_domain_max;

	domain_new.min.x = d.min.x < domain_new.min.x?
		d.min.x : domain_new.min.x;
	domain_new.min.y = d.min.y < domain_new.min.y?
		d.min.y : domain_new.min.y;
	domain_new.max.x = d.max.x > domain_new.max.x?
		d.max.x : domain_new.max.x;
	domain_new.max.y = d.max.y > domain_new.max.y?
		d.max.y : domain_new.max.y;

	domainSet(domain_new);
	}

void XYPlotGtk::backgroundSet(bool light)
	{
	if(light)
		{
		m_background_color=COLORS[ColorID::WHITE];
		m_text_color=COLORS[ColorID::BLACK];
		m_curve_luma=0.4f;
		m_canvas_color=COLORS[ColorID::GRAY];
		}
	else
		{
		m_background_color=COLORS[ColorID::BLACK];
		m_text_color=COLORS[ColorID::WHITE];
		m_curve_luma=0.7f;
		m_canvas_color=COLORS[ColorID::GRAY_DARK];
		}
	update();
	}

void XYPlotGtk::axisXExtents(cairo_t* cr,double x_0,size_t N,double delta_x)
	{
	m_axis_x.clear();
	Curve::Point max_extent{0,0};
	for(size_t k=0;k<=N;++k)
		{
		auto pos=x_0+delta_x*k;
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

void XYPlotGtk::axisYExtents(cairo_t* cr,double y_0,size_t N,double delta_y)
	{
	m_axis_y.clear();
	float max_extent=0;
	for(size_t k=0;k<=N;++k)
		{
		auto pos=y_0+delta_y*k;
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

void XYPlotGtk::ticksUpdate()
	{
	auto domain_new=m_domain_current;
	auto cr=gdk_cairo_create(gtk_widget_get_window(m_canvas));

//	Compute extents for axes given two ticks
	auto N_x=2;
	auto delta_x=(domain_new.max.x - domain_new.min.x)/N_x;
	axisXExtents(cr,domain_new.min.x,N_x,delta_x);

	auto N_y=2;
	auto delta_y=(domain_new.max.y - domain_new.min.y)/N_y;
	axisYExtents(cr,domain_new.min.y,N_y,delta_y);

//	Adjust margins
	windowDomainAdjust(cr,m_width,m_height);

//	Update Y axis
	auto height=m_domain_window.max.y-m_domain_window.min.y;
	N_y=height/(4*m_axis_y.front().extent_y);
	if(N_y==0)
		{return;}
	delta_y=(domain_new.max.y - domain_new.min.y)/N_y;
	m_delta_y=pow(10,round(log10(delta_y)));

//	Fix domain and update m_N_max_y
	domain_new=domainAlignY(domain_new,m_delta_y);
	m_N_max_y=round((domain_new.max.y - domain_new.min.y)/m_delta_y);

//	Adjust margins (again)
	windowDomainAdjust(cr,m_width,m_height);

//	Update X axis
	double delta_x_win;
	double delta_x_min;
	double delta_x_old;
	Curve::Domain domain_old;
	auto x_min=domain_new.min.x;
	auto x_max=domain_new.max.x;
	do
		{
	//	Try a specific number of ticks
		delta_x_old=delta_x;
		delta_x=(x_max - x_min)/N_x;
		delta_x=pow(10,round(log10(delta_x)));

		domain_old=domain_new;
		domain_new=domainAlignX(domain_new,delta_x);
		x_min=domain_new.min.x;
		x_max=domain_new.max.x;
		auto N_x_result=round((x_max - x_min)/delta_x);

	//	See if all ticks fitted, if so, try more ticks
		axisXExtents(cr,x_min,N_x_result,delta_x);
		windowDomainAdjust(cr,m_width,m_height);
		delta_x_min=m_axis_x.back().extent_x+2.5;
		auto x_min_win=mapToDomainX(0,domain_new,m_domain_window);
		auto x_max_win=mapToDomainX(delta_x,domain_new,m_domain_window);
		delta_x_win=x_max_win-x_min_win;
		++N_x;
		}
	while(delta_x_win > delta_x_min);

	m_delta_x=delta_x_old;
	m_N_max_x=N_x;
	cairo_destroy(cr);
	m_domain_current_active=domain_old;
	}

void XYPlotGtk::windowDomainAdjust(cairo_t* cr,int width,int height)
	{
	auto margin_top=m_axis_y.front().extent_y;
	auto margin_bottom=(m_axis_y.end()-2)->extent_y+1.25*m_axis_x.back().extent_y;
	auto margin_right=0.5*(m_axis_x.end()-2)->extent_x;
	auto margin_left=m_axis_y.back().extent_x+2.5;

	m_domain_window.min.x=
		std::max(margin_left,margin_left+0.5*m_axis_x.front().extent_x);
	m_domain_window.min.y=margin_top;
	m_domain_window.max.x=width-margin_right;
	m_domain_window.max.y=height-margin_bottom;
	}

void XYPlotGtk::axisYDraw(cairo_t* cr) const
	{
	auto max_extent=m_axis_y.back().extent_x;
	auto ptr=m_axis_y.data();
	size_t N=m_axis_y.size()-1;
	auto ptr_end=ptr+N;
	size_t k=0;
	while(ptr!=ptr_end)
		{
		auto pos=ptr->value;
		auto point_out=toWindowCoords({0,pos});
		cairo_move_to(cr,max_extent - ptr->extent_x
			,point_out.y + 0.5*ptr->extent_y);
		cairo_show_text(cr,ptr->text);
		++ptr;
		++k;
		}
	}

void XYPlotGtk::axisXDraw(cairo_t* cr) const
	{
	auto max_extent=m_axis_x.back().extent_y;
	auto ptr=m_axis_x.data();
	size_t N=m_axis_x.size();
	auto ptr_end=ptr+N-1;
	auto domain=m_domain_current_active;
	size_t k=0;
	while(ptr!=ptr_end)
		{
		auto pos=ptr->value;
		auto point_out=toWindowCoords({pos,domain.min.y});
		cairo_move_to(cr,point_out.x-0.5*ptr->extent_x
			,point_out.y+1.25*max_extent);
		cairo_show_text(cr,ptr->text);
		++ptr;
		++k;
		}
	}

void XYPlotGtk::curveDraw(cairo_t* cr,const Curve& curve) const
	{
	auto points=curve.pointsGet();
	auto N_points=curve.nPointsGet();
	if(N_points==0)
		{return;}

	ColorSystem color_gtk{ColorHSLA::fromHueAndLuma(curve.colorGet(),m_curve_luma)};
	gdk_cairo_set_source_rgba(cr,color_gtk);
	auto point_out=toWindowCoords(*points);
	++points;
	--N_points;
	cairo_move_to(cr,point_out.x,point_out.y);
	while(N_points!=0)
		{
		point_out=toWindowCoords(*points);
		cairo_line_to(cr,point_out.x,point_out.y);
		++points;
		--N_points;
		}
	cairo_stroke(cr);
	}

gboolean XYPlotGtk::onPaint(GtkWidget* object,cairo_t* cr,void* xyplotgtk)
	{
	XYPlotGtk* _this=(XYPlotGtk*)xyplotgtk;

	auto width = gtk_widget_get_allocated_width (object);
	auto height = gtk_widget_get_allocated_height (object);

	//	Draw background
		{
		ColorSystem color_gtk{_this->m_canvas_color};
		gdk_cairo_set_source_rgba(cr,color_gtk);
		cairo_paint(cr);
		}

	_this->axisXExtents(cr,_this->m_domain_current_active.min.x
		,_this->m_N_max_x,_this->m_delta_x);
	_this->axisYExtents(cr,_this->m_domain_current_active.min.y
		,_this->m_N_max_y,_this->m_delta_y);
	_this->windowDomainAdjust(cr,width,height);

	//	Draw plot area
		{
		auto w=_this->m_domain_window.max.x-_this->m_domain_window.min.x;
		auto h=_this->m_domain_window.max.y-_this->m_domain_window.min.y;
		cairo_rectangle(cr,_this->m_domain_window.min.x
			,_this->m_domain_window.min.y,w,h);
		ColorSystem color_gtk{_this->m_background_color};
		gdk_cairo_set_source_rgba(cr,color_gtk);
		cairo_fill(cr);
		}

	// Draw all curves
		{
		auto ptr_curve=_this->m_curves.data();
		auto ptr_curves_end=ptr_curve+_this->m_curves.size();
		while(ptr_curve!=ptr_curves_end)
			{
		//	TODO: Do not plot points outside view? Showing points outside range
		//		might be a feature
			_this->curveDraw(cr,*ptr_curve);
			++ptr_curve;
			}
		}

	//	Draw all X cursors
		{
		auto ptr_cursor=_this->m_cursors_x.data();
		auto ptr_cursors_end=ptr_cursor+_this->m_cursors_x.size();
		while(ptr_cursor!=ptr_cursors_end)
			{
			auto cursor=*ptr_cursor;
			Curve::Point points[2]=
				{
				 {cursor.position,_this->m_domain_current_active.min.y}
				,{cursor.position,_this->m_domain_current_active.max.y}
				};
		//	Curve tmp{points,2,cursor.hue}
			_this->curveDraw(cr,{points,2,cursor.hue});
			++ptr_cursor;
			}
		}

	//	Draw all Y cursors
		{
		auto ptr_cursor=_this->m_cursors_y.data();
		auto ptr_cursors_end=ptr_cursor+_this->m_cursors_y.size();
		while(ptr_cursor!=ptr_cursors_end)
			{
			auto cursor=*ptr_cursor;
			Curve::Point points[2]=
				{
				 {_this->m_domain_current_active.min.x,cursor.position}
				,{_this->m_domain_current_active.max.x,cursor.position}
				};
		//	Curve tmp{points,2,cursor.hue}
			_this->curveDraw(cr,{points,2,cursor.hue});
			++ptr_cursor;
			}
		}

	//	Draw border around plot area
		{
		ColorSystem color_gtk{_this->m_text_color};
		gdk_cairo_set_source_rgba(cr, color_gtk);

		auto w=_this->m_domain_window.max.x-_this->m_domain_window.min.x;
		auto h=_this->m_domain_window.max.y-_this->m_domain_window.min.y;
		cairo_rectangle(cr,_this->m_domain_window.min.x
			,_this->m_domain_window.min.y,w,h);
		cairo_stroke(cr);
		}

	//	Draw axes
		{
		_this->axisYDraw(cr);
		_this->axisXDraw(cr);
		}

	return FALSE;
	}
