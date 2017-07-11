//@	{
//@	 "targets":[{"name":"memview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "memview.hpp"
#include "container.hpp"
#include "uiutility.hpp"
#include <gtk/gtk.h>
#include <string>
#include <memory>

#include <cstring>
#include <unistd.h>

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
	std::string key;
	std::string value;
	while(true)
		{
		auto ch_in=getc(src);
		if(ch_in=='\r')
			{continue;}

		switch(state_current)
			{
			case State::KEY:
				switch(ch_in)
					{
					case EOF:
						rp(key,value);
						return;
					case ':':
						state_current=State::VALUE;
						break;
					default:
						key+=ch_in;
					}
				break;
			case State::VALUE:
				switch(ch_in)
					{
					case EOF:
						rp(key,value);
						return;
					case '\n':
						rp(key,value);
						key.clear();
						value.clear();
						state_current=State::KEY;
						break;
					default:
						value+=ch_in;
					}
				break;
			}
		}
	}

namespace
	{
	struct FileCloser
		{
		void operator()(FILE* f) const noexcept
			{if(f!=nullptr){fclose(f);}}
		};
	}

gboolean MemView::Impl::update(void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);

	struct MemInfo
		{
		size_t mem_tot;
		size_t mem_avail;
		size_t swap_tot;
		size_t swap_avail;
		} meminfo;
	meminfo=MemInfo{0,0,0,0};

		{
		std::unique_ptr<FILE,FileCloser> memfile(fopen("/proc/meminfo","rb"));
		procfs_read(memfile.get(),[&meminfo](const std::string& key,const std::string& value)
			{
			if(key=="MemTotal")
				{meminfo.mem_tot=static_cast<size_t>( atoll(value.c_str()) );}
			else
			if(key=="MemAvailable")
				{meminfo.mem_avail=static_cast<size_t>( atoll(value.c_str()) );}
			else
			if(key=="SwapTotal")
				{meminfo.swap_tot=static_cast<size_t>( atoll(value.c_str()) );}
			else
			if(key=="SwapFree")
				{meminfo.swap_avail=static_cast<size_t>( atoll(value.c_str()) );}
			});
		}

	auto memtot=meminfo.mem_tot + meminfo.swap_tot;
	self->m_total=static_cast<float>( meminfo.mem_tot - meminfo.mem_avail )/memtot;
	self->m_total_swap=static_cast<float>((meminfo.swap_tot - meminfo.swap_avail))/memtot;
	self->m_mem_ratio=static_cast<float>(meminfo.mem_tot)/static_cast<float>(memtot);

		{
		std::string pidinfo("/proc/");
		pidinfo+=std::to_string(getpid());
		pidinfo+="/status";
		std::unique_ptr<FILE,FileCloser> memfile(fopen(pidinfo.c_str(),"rb"));

		procfs_read(memfile.get(),[self,memtot](const std::string&key,const std::string& value)
			{
			if(key=="VmRSS")
				{
				auto x=static_cast<size_t>(atoll(value.c_str()));
				self->m_self=static_cast<float>(x)/memtot;
				}
			else
			if(key=="VmSwap")
				{
				auto x=static_cast<size_t>(atoll(value.c_str()));
				self->m_self_swap=static_cast<float>(x)/memtot;
				}
			});
		}

	gtk_widget_queue_draw(GTK_WIDGET(self->m_handle));
	return TRUE;
	}

MemView::Impl::Impl(Container& cnt):MemView(*this)
	{
	auto widget=gtk_drawing_area_new();
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	gtk_widget_set_size_request(widget,96,-1);
	m_handle=GTK_DRAWING_AREA(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	update(this);
	m_timer=g_timeout_add(5000,update,this);
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
