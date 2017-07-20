//@	{"targets":[{"name":"progressbar.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "progressbar.hpp"
#include "container.hpp"
#include "focussink.hpp"
#include <gtk/gtk.h>
#include <time.h>
#include <cassert>

using namespace Anja;

static double to_sec(const timespec& t)
	{return t.tv_sec + 1e-9*t.tv_nsec;}

class ProgressBar::Impl:private ProgressBar
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void value(double x) noexcept
			{
			timespec now;
			clock_gettime( CLOCK_MONOTONIC_RAW,&now);
			if( to_sec(now) - to_sec(m_then) > 40e-3 )
				{
				m_then=now;
				gtk_progress_bar_set_fraction(m_handle,x);
				while (g_main_context_pending(NULL))
					{g_main_context_iteration(NULL,FALSE);}
				}
			}

	private:
		GtkProgressBar* m_handle;
		timespec m_then;
	};

ProgressBar::ProgressBar(Container& cnt)
	{m_impl=new Impl(cnt);}

ProgressBar::~ProgressBar()
	{delete m_impl;}

ProgressBar& ProgressBar::value(double x)
	{
	m_impl->value(x);
	return *this;
	}

ProgressBar::Impl::Impl(Container& cnt):ProgressBar(*this)
	{
	auto widget=gtk_progress_bar_new();
	gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(widget),FALSE);
	gtk_widget_set_size_request(widget,320,-1);

	m_handle=GTK_PROGRESS_BAR(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	clock_gettime( CLOCK_MONOTONIC_RAW,&m_then);
	}

ProgressBar::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	g_object_unref(m_handle);
	}
