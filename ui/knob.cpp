//@	{
//@	 "targets":
//@		[{
//@		"name":"knob.o","type":"object","pkgconfig_libs":["gtk+-3.0"]
//@		,"dependencies":
//@			[
//@				 {"ref":"knob-ambient.png","rel":"generated"}
//@				,{"ref":"knob-diffuse.png","rel":"generated"}
//@				,{"ref":"knob-mask.png","rel":"generated"}
//@			]
//@		}]
//@	}

#include "knob.hpp"
#include "container.hpp"
#include "uiutility.hpp"
#include "../common/blob.hpp"
#include "../common/color.hpp"
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

	private:
		int m_id;
		CallbackImpl m_cb;
		void* r_cb_obj;
		cairo_surface_t* m_ambient;
		cairo_surface_t* m_diffuse;
		cairo_surface_t* m_mask;

		GtkDrawingArea* m_handle;
		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
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
	auto widget=gtk_drawing_area_new();
	gtk_widget_add_events(widget,GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK);
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	g_signal_connect(widget,"button-release-event",G_CALLBACK(mouse_up),this);
	gtk_widget_set_size_request(widget,40,40);
	m_handle=GTK_DRAWING_AREA(widget);
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

	return TRUE;
	}

gboolean Knob::Impl::mouse_up(GtkWidget* widget,GdkEventButton* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	if(self->r_cb_obj!=nullptr)
		{self->m_cb(self->r_cb_obj,*self);}
	return TRUE;
	}
