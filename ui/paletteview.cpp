//@	{
//@	 "targets":[{"name":"paletteview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "paletteview.hpp"
#include "container.hpp"
#include "../common/color.hpp"
#include <vector>
#include <gtk/gtk.h>

using namespace Anja;

class PaletteView::Impl:public PaletteView
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void palette(const ColorRGBA* colors,size_t n);


	private:
		std::vector<ColorRGBA> m_colors;
		int cols;
		GtkDrawingArea* m_handle;

		static void size_changed(GtkWidget* widget,GtkAllocation* allocation,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);
	};

PaletteView::PaletteView(Container& cnt)
	{m_impl=new Impl(cnt);}

PaletteView::~PaletteView()
	{delete m_impl;}


PaletteView::Impl::Impl(Container& cnt):PaletteView(*this)
	{
	auto widget=gtk_drawing_area_new();
	gtk_widget_add_events(widget,GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK);
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	g_signal_connect(widget,"button-release-event",G_CALLBACK(mouse_up),this);
	g_signal_connect(widget,"size-allocate", G_CALLBACK(size_changed),this);
	m_handle=GTK_DRAWING_AREA(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	palette(COLORS,ColorID::COLOR_END);
	}

PaletteView::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

PaletteView& PaletteView::palette(const ColorRGBA* colors,size_t n)
	{
	m_impl->palette(colors,n);
	return *this;
	}

void PaletteView::Impl::palette(const ColorRGBA* colors,size_t n)
	{
	m_colors.clear();
	std::for_each(colors,colors + n,[this](const ColorRGBA& c)
		{m_colors.push_back(c);});
	gtk_widget_queue_draw(GTK_WIDGET(m_handle));
	}

gboolean PaletteView::Impl::mouse_up(GtkWidget* widget,GdkEventButton* event,void* obj)
	{return TRUE;}

void PaletteView::Impl::size_changed(GtkWidget* widget,GtkAllocation* allocation,void* obj)
	{
	auto w=gtk_widget_get_allocated_width(widget);
	auto h=gtk_widget_get_allocated_height(widget);

	if(w%24!=0)
		{
		auto self=reinterpret_cast<Impl*>(obj);
		w=24*(w/24 + 1);
		auto n_cols=w/24;
		auto rem=self->m_colors.size()%n_cols==0?0:1;
		auto n_rows=std::max(self->m_colors.size()/n_cols + rem,size_t(1));
		h=24*n_rows;
		gtk_widget_set_size_request(widget,w,h);
		}
	else
		{
		auto self=reinterpret_cast<Impl*>(obj);
		auto n_cols=w/24;
		auto rem=self->m_colors.size()%n_cols==0?0:1;
		auto n_rows=std::max(self->m_colors.size()/n_cols + rem,size_t(1));
		h=24*n_rows;
		gtk_widget_set_size_request(widget,std::max(w-24,24),h);
		}
	}


gboolean PaletteView::Impl::draw(GtkWidget* widget,cairo_t* cr,void* obj)
	{
	auto w=gtk_widget_get_allocated_width(widget);
	auto h=gtk_widget_get_allocated_height(widget);
	if(w%24!=0)
		{;}

	cairo_set_source_rgba(cr,0.5,0.5,0.5,1); //Set neutral background
	cairo_rectangle(cr,0,0,w,h);
	cairo_fill(cr);

	auto self=reinterpret_cast<Impl*>(obj);
	auto n_cols=w/24;
	int pos=0;
	std::for_each(self->m_colors.begin(),self->m_colors.end()
		,[cr,&pos,n_cols](const ColorRGBA& c)
		{
		auto row=pos/n_cols;
		auto col=pos%n_cols;

		cairo_set_source_rgba(cr,c.red,c.green,c.blue,c.alpha);
		cairo_rectangle(cr,1+col*24,1+row*24,22,22);
		cairo_fill(cr);

		++pos;
		});
	


	return TRUE;
	}
