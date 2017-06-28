//@	{
//@  "targets":[{"name":"scrolledwindow.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "scrolledwindow.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class ScrolledWindow::Impl:private ScrolledWindow
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void _add(GtkWidget* handle) noexcept
			{gtk_container_add(GTK_CONTAINER(m_handle),handle);}

		void _show() noexcept
			{gtk_widget_show_all(GTK_WIDGET(m_handle));}

		void _sensitive(bool val)
			{gtk_widget_set_sensitive(GTK_WIDGET(m_handle),val);}

		void* _toplevel() const
			{return gtk_widget_get_toplevel(GTK_WIDGET(m_handle));}

		void border(bool val)
			{gtk_scrolled_window_set_shadow_type(m_handle,val?GTK_SHADOW_IN:GTK_SHADOW_NONE);}

		void directions(uint32_t direction_flags)
			{
			switch(direction_flags)
				{
				case 0:
					gtk_scrolled_window_set_policy(m_handle,GTK_POLICY_NEVER,GTK_POLICY_NEVER);
					break;

				case 1:
					gtk_scrolled_window_set_policy(m_handle,GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
					break;

				case 2:
					gtk_scrolled_window_set_policy(m_handle,GTK_POLICY_AUTOMATIC,GTK_POLICY_NEVER);
					break;

				case 3:
					gtk_scrolled_window_set_policy(m_handle,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
					break;
				}
			}

		Rectangle boundingBox() const noexcept
			{
			auto widget=GTK_WIDGET(m_handle);
			auto size=Vec2<double>
				{
				 static_cast<double>( gtk_widget_get_allocated_width(widget) )
				,static_cast<double>( gtk_widget_get_allocated_height(widget) )
				};
			int xi;
			int yi;
			gtk_widget_translate_coordinates(widget,gtk_widget_get_toplevel(widget)
				,0,0,&xi,&yi);
			auto min=Vec2<double>{static_cast<double>(xi),static_cast<double>(yi)};


			return Rectangle{min,min + size};
			}

	private:
		static void destroy_callback (GtkWidget* object,gpointer user_data);
		GtkScrolledWindow* m_handle;
	};

ScrolledWindow::ScrolledWindow(Container& cnt)
	{m_impl=new ScrolledWindow::Impl(cnt);}

ScrolledWindow::~ScrolledWindow()
	{delete m_impl;}

ScrolledWindow& ScrolledWindow::add(void* handle)
	{
	m_impl->_add(GTK_WIDGET(handle));
	return *this;
	}

ScrolledWindow& ScrolledWindow::show()
	{
	m_impl->_show();
	return *this;
	}

ScrolledWindow& ScrolledWindow::sensitive(bool val)
	{
	m_impl->_sensitive(val);
	return *this;
	}

ScrolledWindow& ScrolledWindow::border(bool val)
	{
	m_impl->border(val);
	return *this;
	}

void* ScrolledWindow::toplevel() const
	{return m_impl->_toplevel();}

ScrolledWindow& ScrolledWindow::directions(uint32_t direction_flags)
	{
	m_impl->directions(direction_flags);
	return *this;
	}

Rectangle ScrolledWindow::boundingBox() const noexcept
	{return m_impl->boundingBox();}

ScrolledWindow::Impl::Impl(Container& cnt):ScrolledWindow(*this)
	{
	auto widget=gtk_scrolled_window_new(NULL,NULL);
	cnt.add(widget);
	g_object_ref_sink(widget);
	m_handle=GTK_SCROLLED_WINDOW(widget);
//	TODO: Separate member function
	gtk_scrolled_window_set_policy(m_handle,GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
	}

ScrolledWindow::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}
