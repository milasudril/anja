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

	private:
		ColorRGBA m_color;
		GtkDrawingArea* m_handle;
		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);
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



ColorView::Impl::Impl(Container& cnt):ColorView(*this),m_color(ColorRGBA(0.5,0.5,0.5,1))
	{
	auto widget=gtk_drawing_area_new();
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	m_handle=GTK_DRAWING_AREA(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

ColorView::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

gboolean ColorView::Impl::draw(GtkWidget* widget,cairo_t* cr,void* obj)
	{
	auto w=gtk_widget_get_allocated_width(widget);
	auto h=gtk_widget_get_allocated_height(widget);
	cairo_set_source_rgba(cr,0.5,0.5,0.5,1); //Set neutral background
	cairo_rectangle(cr,0,0,w,h);
	cairo_fill(cr);

	auto self=reinterpret_cast<Impl*>(obj);
	return TRUE;
	}
