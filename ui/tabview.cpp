//@	{
//@  "targets":[{"name":"tabview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "tabview.hpp"
#include "focussink.hpp"
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

		static gboolean focus_in_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
			{
			auto root=gtk_widget_get_toplevel(widget);
			auto sink=reinterpret_cast<const FocusSink*>( g_object_get_data(G_OBJECT(root),"anja-focus-sink") );
			if(sink!=nullptr)
				{sink->action(sink->object);}
			return TRUE;
			}

	};

TabView::TabView(Container& cnt)
	{;
	m_impl=new TabView::Impl(cnt);
	}

TabView::~TabView()
	{
	delete m_impl;
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
	auto widget=gtk_notebook_new();
	g_signal_connect(widget,"focus-in-event",G_CALLBACK(focus_in_callback),this);
	cnt.add(widget);
	g_object_ref_sink(widget);
	m_handle=GTK_NOTEBOOK(widget);
	gtk_notebook_set_show_border(m_handle,FALSE);
	}

TabView::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	g_object_unref(m_handle);
	}

