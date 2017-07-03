//@	{
//@	 "targets":[{"name":"imageview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "imageview.hpp"
#include "container.hpp"
#include "uiutility.hpp"
#include <gtk/gtk.h>
#include <vector>
#include <cstring>

using namespace Anja;

class ImageView::Impl:private ImageView
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void showPng(const ImageRepository& repo,ImageRepository::IdType id
			,const std::pair<const uint8_t*,const uint8_t*>& data);

		int id() const noexcept
			{return m_id;}

		void callback(CallbackImpl cb,void* cb_obj,int id) noexcept
			{
			m_cb=cb;
			r_cb_obj=cb_obj;
			m_id=id;
			}

		void minHeight(int h)
			{
			m_height_request=h;
			gtk_widget_set_size_request(GTK_WIDGET(m_handle),-1,h);
			}

		void minWidth(int w)
			{gtk_widget_set_size_request(GTK_WIDGET(m_handle),w,-1);}


		void minSize(int w,int h)
			{
			gtk_widget_set_size_request(GTK_WIDGET(m_handle),w,h);
			}

	private:
		int m_id;
		CallbackImpl m_cb;
		void* r_cb_obj;
		const cairo_surface_t* r_img;
		GtkDrawingArea* m_handle;
		int m_height_request;
		bool m_hover;
		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
		static gboolean mouse_move(GtkWidget* object,GdkEvent* event,void* obj);
		static gboolean mouse_leave(GtkWidget* object,GdkEvent* event,void* obj);
	};

ImageView::ImageView(Container& cnt)
	{m_impl=new Impl(cnt);}

ImageView::~ImageView()
	{delete m_impl;}

ImageView& ImageView::showPng(const ImageRepository& repo,ImageRepository::IdType id
	,const std::pair<const uint8_t*,const uint8_t*>& data)
	{
	m_impl->showPng(repo,id,data);
	return *this;
	}

ImageView& ImageView::callback(CallbackImpl cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

int ImageView::id() const noexcept
	{return m_impl->id();}

ImageView& ImageView::minHeight(int h)
	{
	m_impl->minHeight(h);
	return *this;
	}

ImageView& ImageView::minWidth(int w)
	{
	m_impl->minWidth(w);
	return *this;
	}

ImageView& ImageView::minSize(int w,int h)
	{
	m_impl->minSize(w,h);
	return *this;
	}




ImageView::Impl::Impl(Container& cnt):ImageView(*this),r_cb_obj(nullptr)
	,r_img(nullptr),m_hover(0)
	{
	auto widget=gtk_drawing_area_new();
	gtk_widget_add_events(widget,GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK
		|GDK_POINTER_MOTION_MASK|GDK_LEAVE_NOTIFY_MASK);
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	g_signal_connect(widget,"button-release-event",G_CALLBACK(mouse_up),this);
	g_signal_connect(widget,"motion-notify-event",G_CALLBACK(mouse_move),this);
	g_signal_connect(widget,"leave-notify-event",G_CALLBACK(mouse_leave),this);


	m_height_request=48;
	gtk_widget_set_size_request(widget,-1,m_height_request);
	m_handle=GTK_DRAWING_AREA(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

ImageView::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

gboolean ImageView::Impl::draw(GtkWidget* widget,cairo_t* cr,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	auto img=self->r_img;

	auto w_out=static_cast<double>( gtk_widget_get_allocated_width(widget) );
	auto h_out=static_cast<double>( gtk_widget_get_allocated_height(widget) );
	cairo_matrix_t m;
	cairo_get_matrix(cr,&m);

	if(img!=nullptr)
		{
		auto w_win=w_out;
		auto h_win=h_out;

		auto w_in=static_cast<double>( cairo_image_surface_get_width(const_cast<cairo_surface_t*>(img)) );
		auto h_in=static_cast<double>( cairo_image_surface_get_height(const_cast<cairo_surface_t*>(img)) );

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
		cairo_set_source_surface(cr,const_cast<cairo_surface_t*>(img),0,0);
		cairo_paint(cr);
		}
	if(self->m_hover)
		{
		auto bg=ambientColor(widget);
		float light=1.0f  - (luma709(bg)>0.5f?1.0f:0.5f);
		cairo_set_source_rgba(cr,light,light,light,1.0f);
		cairo_set_matrix(cr,&m);
		cairo_set_line_width(cr,2);
		cairo_rectangle(cr,0,0,w_out,h_out);
		cairo_stroke(cr);
		}


	return TRUE;
	}

void ImageView::Impl::showPng(const ImageRepository& repo,ImageRepository::IdType id
	,const std::pair<const uint8_t*,const uint8_t*>& data)
	{
	r_img=reinterpret_cast<const cairo_surface_t*>(repo.getFromPng(id,data));
		{
		auto w_in=static_cast<double>( cairo_image_surface_get_width(const_cast<cairo_surface_t*>(r_img)) );
		auto h_in=static_cast<double>( cairo_image_surface_get_height(const_cast<cairo_surface_t*>(r_img)) );
		auto w=m_height_request*w_in/h_in;
		gtk_widget_set_size_request(GTK_WIDGET(m_handle),w,m_height_request);
		}
	gtk_widget_queue_draw(GTK_WIDGET(m_handle));
	}

gboolean ImageView::Impl::mouse_up(GtkWidget* widget,GdkEventButton* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	if(self->r_cb_obj!=nullptr)
		{self->m_cb(self->r_cb_obj,*self);}
	return TRUE;
	}

gboolean ImageView::Impl::mouse_move(GtkWidget* object,GdkEvent* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	if(self->r_cb_obj!=nullptr && !self->m_hover)
		{
		self->m_hover=1;
		gtk_widget_queue_draw(object);
		return TRUE;
		}
	return FALSE;
	}


gboolean ImageView::Impl::mouse_leave(GtkWidget* object,GdkEvent* event,void* obj)
	{
	reinterpret_cast<Impl*>(obj)->m_hover=0;
	gtk_widget_queue_draw(object);
	return FALSE;
	}
