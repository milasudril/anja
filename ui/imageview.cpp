//@	{
//@	 "targets":[{"name":"imageview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "imageview.hpp"
#include "container.hpp"
#include <gtk/gtk.h>
#include <vector>
#include <cstring>

using namespace Anja;

class ImageView::Impl:public ImageView
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void showPng(const uint8_t* bytes_begin,const uint8_t* bytes_end);

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
		cairo_surface_t* m_img;
		GtkDrawingArea* m_handle;
		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
	};

ImageView::ImageView(Container& cnt)
	{m_impl=new Impl(cnt);}

ImageView::~ImageView()
	{delete m_impl;}

ImageView& ImageView::showPng(const uint8_t* bytes_begin,const uint8_t* bytes_end)
	{
	m_impl->showPng(bytes_begin,bytes_end);
	return *this;
	}

ImageView& ImageView::callback(CallbackImpl cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

int ImageView::id() const noexcept
	{return m_impl->id();}




ImageView::Impl::Impl(Container& cnt):ImageView(*this),r_cb_obj(nullptr)
	,m_img(nullptr)
	{
	auto widget=gtk_drawing_area_new();
	gtk_widget_add_events(widget,GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK);
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	g_signal_connect(widget,"button-release-event",G_CALLBACK(mouse_up),this);
	gtk_widget_set_size_request(widget,32,32);
	m_handle=GTK_DRAWING_AREA(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

ImageView::Impl::~Impl()
	{
	m_impl=nullptr;
	if(m_img!=nullptr)
		{cairo_surface_destroy(m_img);}
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

gboolean ImageView::Impl::draw(GtkWidget* widget,cairo_t* cr,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	auto img=self->m_img;
	if(img!=nullptr)
		{
		auto w_out=static_cast<double>( gtk_widget_get_allocated_width(widget) );
		auto h_out=static_cast<double>( gtk_widget_get_allocated_height(widget) );
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

		cairo_translate(cr,0.5*(w_win - w_out),0.5*(h_win - h_out));
		cairo_scale(cr,scale,scale);
		cairo_set_source_surface(cr,img,0,0);
		cairo_paint(cr);
		}
	return TRUE;
	}

void ImageView::Impl::showPng(const uint8_t* bytes_begin,const uint8_t* bytes_end)
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
		return;
		}
	if(m_img!=nullptr)
		{cairo_surface_destroy(m_img);}
	m_img=temp;
	gtk_widget_queue_draw(GTK_WIDGET(m_handle));
	}

gboolean ImageView::Impl::mouse_up(GtkWidget* widget,GdkEventButton* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	if(self->r_cb_obj!=nullptr)
		{self->m_cb(self->r_cb_obj,*self);}
	return TRUE;
	}
