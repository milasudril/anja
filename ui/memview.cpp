//@	{
//@	 "targets":[{"name":"memview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "memview.hpp"
#include "container.hpp"
#include "uiutility.hpp"
#include <gtk/gtk.h>
#include <string>

#include <sys/sysinfo.h>

using namespace Anja;

class MemView::Impl:private MemView
	{
	public:
		Impl(Container& cnt);
		~Impl();


	private:
		GtkDrawingArea* m_handle;
		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);
		static gboolean update(void* obj);

		float m_self;
		float m_total;
		float m_self_swap;
		float m_total_swap;
		float m_mem_ratio;
		guint m_timer;
	};

MemView::MemView(Container& cnt)
	{m_impl=new Impl(cnt);}

MemView::~MemView()
	{delete m_impl;}


template<class RecProc>
static void procfs_read(FILE* src,RecProc&& rp)
	{
	enum class State:int{KEY,VALUE};
	auto state_current=State::KEY;
	while(true)
		{
		auto ch_in=getc(src);
		if(ch_in=='\r')
			{continue;}

		switch(state_current)
			{
			case KEY:
				switch(ch_in)
					{
					case EOF:
						break;
					case ':':
						break;
					default:
						break;
					}
				break;
			case VALUE:
				switch(ch_in)
					{
					case EOF:
						break;
					case '\n':
						break;
					default:
						break;
					}
				break;
			}
		}
	}

gboolean MemView::Impl::update(void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);

	struct sysinfo info;
	sysinfo(&info);

	auto memtot=info.totalram + info.totalswap;
	self->m_mem_ratio=static_cast<float>(info.totalram)/static_cast<float>(memtot);
	self->m_total=static_cast<float>( info.totalram - info.bufferram - info.freeram)/memtot;

	fprintf(stderr,"%ld %ld %ld\n",info.bufferram,info.freeram,info.bufferram+info.freeram);

	self->m_self=0.0f;

	gtk_widget_queue_draw(GTK_WIDGET(self->m_handle));

	return TRUE;
	}

MemView::Impl::Impl(Container& cnt):MemView(*this)
	{
	auto widget=gtk_drawing_area_new();
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	gtk_widget_set_size_request(widget,128,-1);
	m_handle=GTK_DRAWING_AREA(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	m_timer=g_timeout_add(1000,update,this);

//TODO query procfs
	m_self=0.25f;
	m_total=0.5f;
	m_self_swap=0.125f/2;
	m_total_swap=0.25f/2;
	m_mem_ratio=0.75f;
	}

MemView::Impl::~Impl()
	{
	g_source_remove(m_timer);
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

static inline int dark_check(GtkWidget* widget)
	{
	auto color=ambientColor(widget);
	auto luma=luma709(color);
	return luma < 0.5f;
	}

gboolean MemView::Impl::draw(GtkWidget* widget,cairo_t* cr,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);

	auto w_out=static_cast<double>( gtk_widget_get_allocated_width(widget) );
	auto h_out=static_cast<double>( gtk_widget_get_allocated_height(widget) );

	auto dark=dark_check(widget);
		{
		auto background=dark?ColorRGBA(0,0,0,1):ColorRGBA(1,1,1,1);

		cairo_set_source_rgba(cr,background.red,background.green,background.blue,background.alpha);
		cairo_rectangle(cr,0,0,w_out,h_out);
		cairo_fill(cr);
		}

		{
		ColorRGBA color(ColorHSLA::fromHueAndLuma(0.33,dark?0.7:0.4));
		cairo_set_source_rgba(cr,color.red,color.green,color.blue,0.5);
		cairo_rectangle(cr,0,0,self->m_total*w_out,h_out);
		cairo_fill(cr);

		cairo_set_source_rgba(cr,color.red,color.green,color.blue,1.0);
		cairo_rectangle(cr,0,0,self->m_self*w_out,h_out);
		cairo_fill(cr);
		}

		{
		ColorRGBA color(ColorHSLA::fromHueAndLuma(0.0,dark?0.7:0.4));
		cairo_set_source_rgba(cr,color.red,color.green,color.blue,0.5);
		cairo_rectangle(cr,w_out*self->m_mem_ratio,0,self->m_total_swap*w_out,h_out);
		cairo_fill(cr);

		cairo_set_source_rgba(cr,color.red,color.green,color.blue,1.0);
		cairo_rectangle(cr,w_out*self->m_mem_ratio,0,self->m_self_swap*w_out,h_out);
		cairo_fill(cr);
		}

		{
		auto border=!dark?ColorRGBA(0,0,0,1):ColorRGBA(1,1,1,1);
		cairo_set_source_rgba(cr,border.red,border.green,border.blue,border.alpha);
		cairo_set_line_width(cr,2);
		cairo_rectangle(cr,0,0,w_out,h_out);
		cairo_stroke(cr);
		}

	return TRUE;
	}
