//@	{
//@	 "targets":
//@		[{
//@		"name":"knob.o","type":"object","pkgconfig_libs":["gtk+-3.0"]
//@		,"cxxoptions":{"cflags_extra":["fno-lto"]}
//@		,"dependencies":
//@			[
//@				 {"ref":"knob-ambient.png","rel":"generated"}
//@				,{"ref":"knob-diffuse.png","rel":"generated"}
//@				,{"ref":"knob-mask.png","rel":"generated"}
//@			]
//@		}]
//@	}

//TODO: Add mousewheel
//TODO: Add arrow keys

#include "knob.hpp"
#include "container.hpp"
#include "uiutility.hpp"
#include "../common/blob.hpp"
#include "../common/color.hpp"
#include "../common/vec2.hpp"
#include <gtk/gtk.h>
#include <maike/targetinclude.hpp>
#include <vector>
#include <cstring>

ANJA_BLOB(uint8_t,knob_ambient,MAIKE_TARGET(knob-ambient.png));
ANJA_BLOB(uint8_t,knob_diffuse,MAIKE_TARGET(knob-diffuse.png));
ANJA_BLOB(uint8_t,knob_mask,MAIKE_TARGET(knob-mask.png));

using namespace Anja;

class Knob::Impl:public Knob
	{
	public:
		Impl(Container& cnt);
		~Impl();

		int id() const noexcept
			{return m_id;}

		void callback(CallbackImpl cb,void* cb_obj,int id) noexcept
			{
			m_cb=cb;
			r_cb_obj=cb_obj;
			m_id=id;
			}

		void value(double x)
			{
			m_value=std::max(0.0,std::min(x,1.0));
			gtk_widget_queue_draw(GTK_WIDGET(m_handle));
			}

		double value() const noexcept
			{return m_value;}

	private:
		int m_id;
		CallbackImpl m_cb;
		void* r_cb_obj;
		double m_value;
		bool m_grabbed;
		cairo_surface_t* m_ambient;
		cairo_surface_t* m_diffuse;
		cairo_surface_t* m_mask;

		GtkDrawingArea* m_handle;
		static gboolean draw(GtkWidget* widget,cairo_t* cr,void* obj);
		static gboolean mouse_move(GtkWidget* object,GdkEventMotion* event,void* obj);
		static gboolean mouse_down(GtkWidget* object,GdkEventButton* event,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
		static gboolean mousewheel(GtkWidget* widget,GdkEvent* event,void* obj);

		void value_update(Vec2 position);
	};

Knob::Knob(Container& cnt)
	{m_impl=new Impl(cnt);}

Knob::~Knob()
	{delete m_impl;}

Knob& Knob::callback(CallbackImpl cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

int Knob::id() const noexcept
	{return m_impl->id();}

double Knob::value() const noexcept
	{return m_impl->value();}

Knob& Knob::value(double x)
	{
	m_impl->value(x);
	return *this;
	}




static cairo_surface_t* image_load(const uint8_t* bytes_begin,const uint8_t* bytes_end)
	{
	typedef std::pair<const uint8_t*,const uint8_t*> ReadPair;
	ReadPair read_pair{bytes_begin,bytes_end};
	auto temp=cairo_image_surface_create_from_png_stream([]
		(void* cb_obj,unsigned char *data,unsigned int length)
		{
		auto rp=reinterpret_cast<ReadPair*>(cb_obj);
		auto n=std::min(static_cast<unsigned int>(rp->second - rp->first),length);
		memcpy(data,rp->first,n);
		rp->first+=n;
		return CAIRO_STATUS_SUCCESS;
		},&read_pair);
	if(cairo_surface_status(temp)!=CAIRO_STATUS_SUCCESS)
		{
		cairo_surface_destroy(temp);
		return nullptr;
		}
	return temp;
	}

Knob::Impl::Impl(Container& cnt):Knob(*this),r_cb_obj(nullptr)
	{
	m_ambient=image_load(knob_ambient_begin,knob_ambient_end);
	m_mask=image_load(knob_mask_begin,knob_mask_end);
	m_diffuse=image_load(knob_diffuse_begin,knob_diffuse_end);
	m_value=0;
	m_grabbed=0;
	auto widget=gtk_drawing_area_new();
	gtk_widget_set_can_focus(widget,TRUE);
	gtk_widget_add_events(widget,GDK_POINTER_MOTION_MASK|GDK_BUTTON_PRESS_MASK
		|GDK_BUTTON_RELEASE_MASK|GDK_KEY_PRESS_MASK|GDK_KEY_RELEASE_MASK
		|GDK_SCROLL_MASK);
	gtk_widget_set_size_request(widget,40,40);
	m_handle=GTK_DRAWING_AREA(widget);
	g_signal_connect(m_handle,"draw",G_CALLBACK(draw),this);
	g_signal_connect(m_handle,"motion-notify-event",G_CALLBACK(mouse_move),this);
	g_signal_connect(m_handle,"button-press-event",G_CALLBACK(mouse_down),this);
	g_signal_connect(m_handle,"button-release-event",G_CALLBACK(mouse_up),this);
//TODO:	g_signal_connect(m_handle,"scroll-event", G_CALLBACK(mousewheel),this);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

Knob::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	if(m_diffuse!=nullptr)
		{cairo_surface_destroy(m_diffuse);}
	if(m_ambient!=nullptr)
		{cairo_surface_destroy(m_mask);}
	if(m_mask!=nullptr)
		{cairo_surface_destroy(m_ambient);}
	}

static void image_render(cairo_t* cr,cairo_surface_t* img,double w_out,double h_out)
	{
	auto w_win=w_out;
	auto h_win=h_out;

	auto w_in=static_cast<double>( cairo_image_surface_get_width(img) );
	auto h_in=static_cast<double>( cairo_image_surface_get_height(img) );

	auto scale=[w_in,&w_out,h_in,&h_out]()
		{
		auto r_out=w_out/h_out;
		auto r_in=w_in/h_in;
		if(r_out>r_in)
			{
			w_out=h_out*r_in;
			return w_out/w_in;
			}
		h_out=w_out/r_in;
		return h_out/h_in;
		}();

	cairo_save(cr);
	cairo_translate(cr,0.5*(w_win - w_out),0.5*(h_win - h_out));
	cairo_scale(cr,scale,scale);
	cairo_set_source_surface(cr,img,0,0);
	cairo_paint(cr);
	cairo_restore(cr);
	}

static constexpr double pi=std::acos(-1);

static double value_to_angle(double val)
	{
	return (1-val)*(-pi/6.0) + val*7.0*pi/6.0;
	}

static double angle_to_value(double angle)
	{
	return (angle - (-pi/6.0))/(7.0*pi/6.0 - (-pi/6.0));
	}

static Vec2 angle_to_pos(double r,double angle,Vec2 O)
	{return r*Vec2{cos(angle),sin(angle)} + O;}

static double pos_to_angle(Vec2 pos,Vec2 O)
	{
	pos-=O;
	return atan2(-pos.x(),pos.y()) + 0.5*pi;
	}

static Vec2 flip_y(Vec2 v,double h)
	{return Vec2{v.x(),h-v.y()};}

gboolean Knob::Impl::draw(GtkWidget* widget,cairo_t* cr,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	auto w=static_cast<double>( gtk_widget_get_allocated_width(widget) );
	auto h=static_cast<double>( gtk_widget_get_allocated_height(widget) );


	auto bg=ambientColor(widget);
	cairo_push_group(cr);
		cairo_set_source_rgba(cr,bg.red,bg.green,bg.blue,1);
		cairo_rectangle(cr,0,0,w,h);
		cairo_fill(cr);
		cairo_set_operator(cr,CAIRO_OPERATOR_DEST_ATOP);
		image_render(cr,self->m_mask,w,h);
	cairo_pop_group_to_source(cr);
	cairo_set_operator(cr,CAIRO_OPERATOR_OVER);
	cairo_paint(cr);

	cairo_set_operator(cr,CAIRO_OPERATOR_MULTIPLY);
	image_render(cr,self->m_ambient,w,h);
	cairo_set_operator(cr,CAIRO_OPERATOR_ADD);
	image_render(cr,self->m_diffuse,w,h);


	float light=luma709(bg)>0.5f?1.0f:0.0f;
	float i=light + (1.0f - light)*0.4f;

	cairo_push_group(cr);
		cairo_set_source_rgba(cr,i,i,i,1);
		cairo_rectangle(cr,0,0,w,h);
		cairo_fill(cr);
		cairo_set_operator(cr,CAIRO_OPERATOR_DEST_ATOP);
		image_render(cr,self->m_mask,w,h);
	cairo_pop_group_to_source(cr);
	cairo_set_operator(cr,CAIRO_OPERATOR_MULTIPLY);
	cairo_paint(cr);

	GdkRGBA c;
	auto context=gtk_widget_get_style_context(widget);
	gtk_style_context_get_color(context,GTK_STATE_FLAG_NORMAL,&c);
	cairo_set_source_rgba(cr,c.red,c.green,c.blue,c.alpha);
	auto r0=std::min(w,h);
	auto o=flip_y(angle_to_pos(0.28*r0,value_to_angle(1.0 - self->m_value),0.5*Vec2{w,h}),h);
	cairo_arc(cr,o.x(),o.y(),0.04*r0,0,2.0*pi);
	cairo_set_operator(cr,CAIRO_OPERATOR_OVER);
	cairo_fill(cr);

	return TRUE;
	}

gboolean Knob::Impl::mouse_up(GtkWidget* widget,GdkEventButton* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	self->m_grabbed=0;
	return TRUE;
	}

void Knob::Impl::value_update(Vec2 pos)
	{
	auto w=static_cast<double>(gtk_widget_get_allocated_width(GTK_WIDGET(m_handle)));
	auto h=static_cast<double>(gtk_widget_get_allocated_height(GTK_WIDGET(m_handle)));
	pos=flip_y(pos,h);
	auto angle=pos_to_angle(pos,0.5*Vec2{w,h});
	m_value=1.0 - std::max(0.0,std::min(angle_to_value(angle),1.0));
	gtk_widget_queue_draw(GTK_WIDGET(m_handle));
	if(r_cb_obj!=nullptr)
		{m_cb(r_cb_obj,*this);}
	}

gboolean Knob::Impl::mouse_down(GtkWidget* widget,GdkEventButton* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	self->m_grabbed=1;
	self->value_update(Vec2{event->x,event->y});
	return TRUE;
	}

gboolean Knob::Impl::mouse_move(GtkWidget* widget,GdkEventMotion* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	if(self->m_grabbed)
		{
		self->value_update(Vec2{event->x,event->y});
		return TRUE;
		}
	return FALSE;
	}
