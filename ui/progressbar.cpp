//@	{"targets":[{"name":"progressbar.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "progressbar.hpp"
#include "container.hpp"
#include "focussink.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class ProgressBar::Impl:private ProgressBar
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void value(double x) noexcept
			{gtk_progress_bar_set_fraction(m_handle,x);}

	private:
		GtkProgressBar* m_handle;
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
	gtk_widget_set_size_request(widget,96,32);

	m_handle=GTK_PROGRESS_BAR(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

ProgressBar::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}
