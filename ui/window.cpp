//@	{
//@  "targets":[{"name":"window.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "window.hpp"
#include <gtk/gtk.h>
#include <string>

using namespace Anja;

class Window::Impl:private Window
	{
	public:
		Impl(const char* ti,Container* owner);
		~Impl();

		const char* title() const noexcept
			{return m_title.c_str();}

		void title(const char* title_new)
			{
			gtk_window_set_title(m_handle,title_new);
			m_title=title_new;
			}

		void _add(GtkWidget* handle)
			{gtk_container_add(GTK_CONTAINER(m_handle),handle);}

		void _show()
			{gtk_widget_show_all(GTK_WIDGET(m_handle));}

		void _sensitive(bool val)
			{gtk_widget_set_sensitive(GTK_WIDGET(m_handle),val);}

		void* _toplevel() const
			{return m_handle;}

		void callback(Callback cb,void* cb_obj,int id)
			{
			r_cb=cb;
			r_cb_obj=cb_obj;
			m_id=id;
			}

		int id() const noexcept
			{return m_id;}

		void modal(bool state)
			{gtk_window_set_modal(m_handle,state);}

	private:
		static gboolean delete_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data);
		int m_id;
		Callback r_cb;
		void* r_cb_obj;
		GtkWindow* m_handle;
		std::string m_title;
	};

Window::Window(const char* title,Container* owner)
	{m_impl=new Impl(title,owner);}

Window::~Window()
	{delete m_impl;}

const char* Window::title() const noexcept
	{return m_impl->title();}

Window& Window::title(const char* title_new)
	{
	m_impl->title(title_new);
	return *this;
	}

Window& Window::add(void* handle)
	{
	m_impl->_add(GTK_WIDGET(handle));
	return *this;
	}

Window& Window::show()
	{
	m_impl->_show();
	return *this;
	}

Window& Window::sensitive(bool val)
	{
	m_impl->_sensitive(val);
	return *this;
	}

void* Window::toplevel() const
	{return m_impl->_toplevel();}

int Window::id() const noexcept
	{return m_impl->id();}

Window& Window::callback(Callback cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

Window& Window::modal(bool state)
	{
	m_impl->modal(state);
	return *this;
	}


Window::Impl::Impl(const char* ti,Container* owner):Window(*this),m_id(0),r_cb(nullptr)
	{
	auto widget=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(widget,"delete-event",G_CALLBACK(delete_callback),this);
	m_handle=GTK_WINDOW(widget);
	title(ti);
	if(owner!=nullptr)
		{gtk_window_set_transient_for(m_handle,GTK_WINDOW(owner->toplevel()));}
	}

Window::Impl::~Impl()
	{
	m_impl=nullptr;
	r_cb=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

gboolean Window::Impl::delete_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto self=reinterpret_cast<Impl*>(user_data);
	if(self->r_cb!=nullptr)
		{self->r_cb(self->r_cb_obj,*self);}
	return TRUE;
	}
