//@	{"targets":[{"name":"separator.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "separator.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class Separator::Impl:private Separator
	{
	public:
		Impl(Container& cnt,bool vertical);
		~Impl();

	private:
		GtkSeparator* m_handle;
	};

Separator::Separator(Container& cnt,bool vertical) noexcept
	{m_impl=new Impl(cnt,vertical);}

Separator::~Separator()
	{delete m_impl;}

Separator::Impl::Impl(Container& cnt,bool vertical):Separator(*this)
	{
	printf("Separator %p ctor\n",this);

	auto widget=gtk_separator_new(vertical?GTK_ORIENTATION_VERTICAL
		:GTK_ORIENTATION_HORIZONTAL);

	m_handle=GTK_SEPARATOR(widget);
	g_object_ref_sink(m_handle);
	cnt.add(widget);
	}

Separator::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	printf("Separator %p dtor\n",this);
	}
