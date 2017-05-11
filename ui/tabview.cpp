//@	{
//@  "targets":[{"name":"tabview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "tabview.hpp"
#include <gtk/gtk.h>
#include <string>

using namespace Anja;

class TabView::Impl:private TabView
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void _add(GtkWidget* handle) noexcept
			{
			auto label=gtk_label_new(m_tab_title.c_str());
			gtk_widget_set_margin_start(handle,4);
			gtk_widget_set_margin_end(handle,4);
			gtk_widget_set_margin_top(handle,4);
			gtk_widget_set_margin_bottom(handle,4);
			gtk_widget_show(handle);
			gtk_widget_show(label);
			gtk_notebook_append_page(m_handle,handle,label);
			}

		void _show() noexcept
			{gtk_widget_show_all(GTK_WIDGET(m_handle));}

		void _sensitive(bool val)
			{gtk_widget_set_sensitive(GTK_WIDGET(m_handle),val);}

		void* _toplevel() const
			{return gtk_widget_get_toplevel(GTK_WIDGET(m_handle));}

		void tabTitle(const char* t)
			{m_tab_title=std::string(t);}

		void activate(int index)
			{gtk_notebook_set_current_page(m_handle,index);}

	private:
		static void destroy_callback (GtkWidget* object,gpointer user_data);
		GtkNotebook* m_handle;
		std::string m_tab_title;
	};

TabView::TabView(Container& cnt)
	{
	printf("TabView %p ctor\n",this);
	m_impl=new TabView::Impl(cnt);
	}

TabView::~TabView()
	{
	delete m_impl;
	printf("TabView %p dtor\n",this);
	}

TabView& TabView::add(void* handle)
	{
	m_impl->_add(GTK_WIDGET(handle));
	return *this;
	}

TabView& TabView::show()
	{
	m_impl->_show();
	return *this;
	}

TabView& TabView::sensitive(bool val)
	{
	m_impl->_sensitive(val);
	return *this;
	}

void* TabView::toplevel() const
	{return m_impl->_toplevel();}

TabView& TabView::tabTitle(const char* t)
	{
	m_impl->tabTitle(t);
	return *this;
	}

TabView& TabView::activate(int index) noexcept
	{
	m_impl->activate(index);
	return *this;
	}



TabView::Impl::Impl(Container& cnt):TabView(*this)
	{
	printf("TabView::Impl %p ctor\n",this);
	auto widget=gtk_notebook_new();
	cnt.add(widget);
	g_object_ref_sink(widget);
	m_handle=GTK_NOTEBOOK(widget);
	}

TabView::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	printf("TabView::Impl %p dtor\n",this);
	}

