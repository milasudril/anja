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

		void palette(const ColorRGBA* colors_begin,const ColorRGBA* colors_end);

		int id() const noexcept
			{return m_id;}

		void callback(CallbackImpl cb,void* cb_obj,int id) noexcept
			{
			m_cb=cb;
			r_cb_obj=cb_obj;
			m_id=id;
			}

		int selection() const noexcept
			{return m_index_sel;}

		void selection(int index)
			{
			m_index_sel=std::max(0,std::min(index,static_cast<int>(m_colors.size())));
			gtk_widget_queue_draw(GTK_WIDGET(m_handle));
			}

		const ColorRGBA& color(int index) const noexcept
			{return m_colors[index];}

		void color(int index,const ColorRGBA& color) noexcept
			{
			m_colors[index]=color;
			gtk_widget_queue_draw(GTK_WIDGET(m_handle));
			}

		const ColorRGBA* paletteBegin() const noexcept
			{return m_colors.data();}

		const ColorRGBA* paletteEnd() const noexcept
			{return m_colors.data() + m_colors.size();}

	private:
		int m_id;
		CallbackImpl m_cb;
		void* r_cb_obj;

		std::vector<ColorRGBA> m_colors;
		int m_n_cols;
		int m_index_sel;
		GtkDrawingArea* m_handle;

		static void size_changed(GtkWidget* widget,GtkAllocation* allocation,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);
	};

PaletteView::PaletteView(Container& cnt)
	{m_impl=new Impl(cnt);}

PaletteView::~PaletteView()
	{delete m_impl;}

int PaletteView::id() const noexcept
	{return m_impl->id();}

int PaletteView::selection() const noexcept
	{return m_impl->selection();}

PaletteView& PaletteView::selection(int index)
	{
	m_impl->selection(index);
	return *this;
	}

PaletteView& PaletteView::palette(const ColorRGBA* colors_begin
	,const ColorRGBA* colors_end)
	{
	m_impl->palette(colors_begin,colors_end);
	return *this;
	}

const ColorRGBA& PaletteView::color(int index) const noexcept
	{return m_impl->color(index);}

PaletteView& PaletteView::callback(CallbackImpl cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

PaletteView& PaletteView::color(int index,const ColorRGBA& color)
	{
	m_impl->color(index,color);
	return *this;
	}

const ColorRGBA* PaletteView::paletteBegin() const noexcept
	{return m_impl->paletteBegin();}

const ColorRGBA* PaletteView::paletteEnd() const noexcept
	{return m_impl->paletteEnd();}


PaletteView::Impl::Impl(Container& cnt):PaletteView(*this),m_id(0),m_cb(nullptr)
	,r_cb_obj(nullptr),m_index_sel(0)
	{
	auto widget=gtk_drawing_area_new();
	gtk_widget_add_events(widget,GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK);
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	g_signal_connect(widget,"button-release-event",G_CALLBACK(mouse_up),this);
	g_signal_connect(widget,"size-allocate", G_CALLBACK(size_changed),this);
	m_handle=GTK_DRAWING_AREA(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	palette(COLORS,COLORS + ColorID::COLOR_END);
	}

PaletteView::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

void PaletteView::Impl::palette(const ColorRGBA* colors_begin,const ColorRGBA* colors_end)
	{
	m_colors.clear();
	int k=0;
	std::for_each(colors_begin,colors_end,[this,&k](const ColorRGBA& c)
		{
		m_colors.push_back(c);
		++k;
		});
	auto n=static_cast<int>(colors_end - colors_end);
	m_index_sel=std::min(m_index_sel,n-1);
	gtk_widget_queue_draw(GTK_WIDGET(m_handle));
	}

gboolean PaletteView::Impl::mouse_up(GtkWidget* widget,GdkEventButton* event,void* obj)
	{
	auto pos_x=static_cast<int>( event->x/24 );
	auto pos_y=static_cast<int>( event->y/24 );
	auto self=reinterpret_cast<Impl*>(obj);
	if(pos_x>=0 && pos_y>=0)
		{
		auto index=std::min(size_t(pos_y*self->m_n_cols + pos_x)
			,self->m_colors.size()-1);
		self->m_index_sel=index;
		gtk_widget_queue_draw(GTK_WIDGET(widget));
		if(self->r_cb_obj!=nullptr)
			{self->m_cb(self->r_cb_obj,*self);}
		}

	return TRUE;
	}

void PaletteView::Impl::size_changed(GtkWidget* widget,GtkAllocation* allocation,void* obj)
	{
	auto w=gtk_widget_get_allocated_width(widget);
	auto h=gtk_widget_get_allocated_height(widget);

	if(w%24!=0)
		{
		auto self=reinterpret_cast<Impl*>(obj);
		w=24*(w/24 + 1);
		auto n_cols=w/24;
		self->m_n_cols=n_cols;
		auto rem=self->m_colors.size()%n_cols==0?0:1;
		auto n_rows=std::max(self->m_colors.size()/n_cols + rem,size_t(1));
		h=24*n_rows;
		gtk_widget_set_size_request(widget,w,h);
		}
	else
		{
		auto self=reinterpret_cast<Impl*>(obj);
		auto n_cols=w/24;
		self->m_n_cols=n_cols;
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

	if(self->m_index_sel!=-1)
		{
		auto row_sel=self->m_index_sel/n_cols;
		auto col_sel=self->m_index_sel%n_cols;
		auto color_sel=self->m_colors[self->m_index_sel];
		cairo_set_source_rgba(cr,1.0 - color_sel.red
			,1.0 - color_sel.green,1.0 - color_sel.blue,1.0);
		cairo_rectangle(cr,col_sel*24,row_sel*24,24,24);
		cairo_set_line_width(cr,2);
		cairo_stroke(cr);
		}
	return TRUE;
	}
