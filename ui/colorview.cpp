//@	{
//@	 "targets":[{"name":"colorview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "colorview.hpp"
#include "container.hpp"
#include "../common/color.hpp"
#include <vector>
#include <gtk/gtk.h>

using namespace Anja;

class ColorView::Impl:public ColorView
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void color(const ColorRGBA& color)
			{
			m_color=color;
			gtk_widget_queue_draw(GTK_WIDGET(m_handle));
			}

		const ColorRGBA& color() const noexcept
			{return m_color;}

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
		ColorRGBA m_color;
		GtkDrawingArea* m_handle;
		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
	};

ColorView::ColorView(Container& cnt)
	{m_impl=new Impl(cnt);}

ColorView::~ColorView()
	{delete m_impl;}

ColorView& ColorView::color(const ColorRGBA& c)
	{
	m_impl->color(c);
	return *this;
	}

const ColorRGBA& ColorView::color() const noexcept
	{return m_impl->color();}

ColorView& ColorView::callback(CallbackImpl cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

int ColorView::id() const noexcept
	{return m_impl->id();}




ColorView::Impl::Impl(Container& cnt):ColorView(*this),r_cb_obj(nullptr)
	,m_color(ColorRGBA(0.5,0.5,0.5,1))
	{
	auto widget=gtk_drawing_area_new();
	gtk_widget_add_events(widget,GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK);
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	g_signal_connect(widget,"button-release-event",G_CALLBACK(mouse_up),this);
	gtk_widget_set_size_request(widget,24,24);
	m_handle=GTK_DRAWING_AREA(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

ColorView::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	g_object_unref(m_handle);
	}

gboolean ColorView::Impl::draw(GtkWidget* widget,cairo_t* cr,void* obj)
	{
	auto w=gtk_widget_get_allocated_width(widget);
	auto h=gtk_widget_get_allocated_height(widget);
	cairo_set_source_rgba(cr,0.5,0.5,0.5,1); //Set neutral background
	cairo_rectangle(cr,0,0,w,h);
	cairo_fill(cr);

	auto self=reinterpret_cast<Impl*>(obj);
	auto c=self->m_color;
	cairo_set_source_rgba(cr,c.red,c.green,c.blue,c.alpha);
	cairo_rectangle(cr,2,2,w-4,h-4);
	cairo_fill(cr);

	return TRUE;
	}

gboolean ColorView::Impl::mouse_up(GtkWidget* widget,GdkEventButton* event,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	if(self->r_cb_obj!=nullptr)
		{self->m_cb(self->r_cb_obj,*self);}
	return TRUE;
	}
