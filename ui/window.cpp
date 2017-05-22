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
			{
			gtk_widget_set_margin_start(handle,4);
			gtk_widget_set_margin_end(handle,4);
			gtk_widget_set_margin_top(handle,4);
			gtk_widget_set_margin_bottom(handle,4);
			gtk_container_add(GTK_CONTAINER(m_handle),handle);
			}

		void _show()
			{gtk_widget_show_all(GTK_WIDGET(m_handle));}

		void _sensitive(bool val)
			{gtk_widget_set_sensitive(GTK_WIDGET(m_handle),val);}

		void* _toplevel() const
			{return m_handle;}

		void callback(const Vtable& vt,void* cb_obj,int id)
			{
			m_vt=vt;
			r_cb_obj=cb_obj;
			m_id=id;
			}

		int id() const noexcept
			{return m_id;}

		void modal(bool state)
			{gtk_window_set_modal(m_handle,state);}

		void fullscreen(bool state)
			{
			if(state)
				{gtk_window_fullscreen(m_handle);}
			else
				{gtk_window_unfullscreen(m_handle);}
			}

	private:
		static gboolean delete_callback(GtkWidget* widget,GdkEvent* event,void* user_data);
		static gboolean key_down(GtkWidget*widget,GdkEvent* event,void* user_data);
		static gboolean key_up(GtkWidget* widget,GdkEvent* event,void* user_data);
		static gboolean mouse_down(GtkWidget* object,GdkEventButton* event,void* user_data);

		int m_id;
		void* r_cb_obj;
		Vtable m_vt;
		GtkWindow* m_handle;
		GtkWidget* r_focus_old;
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

Window& Window::callback(const Vtable& vt,void* cb_obj,int id)
	{
	m_impl->callback(vt,cb_obj,id);
	return *this;
	}

Window& Window::modal(bool state)
	{
	m_impl->modal(state);
	return *this;
	}

Window& Window::fullscreen(bool state)
	{
	m_impl->fullscreen(state);
	return *this;
	}



Window::Impl::Impl(const char* ti,Container* owner):Window(*this),m_id(0)
	,r_cb_obj(nullptr),r_focus_old(nullptr)
	{
	auto widget=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(widget,"delete-event",G_CALLBACK(delete_callback),this);
	g_signal_connect(widget,"key-press-event",G_CALLBACK(key_down),this);
	g_signal_connect(widget,"key-release-event",G_CALLBACK(key_up),this);
	g_signal_connect(widget,"button-press-event",G_CALLBACK(mouse_down),this);

	m_handle=GTK_WINDOW(widget);
	title(ti);
	if(owner!=nullptr)
		{gtk_window_set_transient_for(m_handle,GTK_WINDOW(owner->toplevel()));}
	}

Window::Impl::~Impl()
	{
	m_impl=nullptr;
	r_cb_obj=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

gboolean Window::Impl::delete_callback(GtkWidget* widget,GdkEvent* event,void* user_data)
	{
	auto self=reinterpret_cast<Impl*>(user_data);
	if(self->r_cb_obj!=nullptr)
		{self->m_vt.closing(self->r_cb_obj,*self,self->m_id);}
	return TRUE;
	}

gboolean Window::Impl::key_down(GtkWidget* widget,GdkEvent* event,void* user_data)
	{
	auto self=reinterpret_cast<Impl*>(user_data);
	auto& key=event->key;
	auto scancode=key.hardware_keycode - 8;
	auto keymask=keymaskFromSystem(key.state);
#ifndef __linux__
	#waring "Scancode key offset is not tested. Pressing esc should print 1"
	printf("%d\n",key.hardware_keycode - 8);
#endif
	auto w=gtk_window_get_focus(GTK_WINDOW(widget));
	if(w!=NULL)
		{
		self->r_focus_old=w;
		switch(scancode)
			{
			case 1: //ESC
				gtk_window_set_focus(GTK_WINDOW(widget),NULL);
				if(self->r_cb_obj!=nullptr)
					{self->m_vt.key_down(self->r_cb_obj,*self,scancode,keymask,self->m_id);}
				break;
			case 28: //RETURN
				if(gtk_window_get_transient_for(GTK_WINDOW(widget))!=NULL) //Dialog box
					{
					gtk_window_set_focus(GTK_WINDOW(widget),NULL);
					if(self->r_cb_obj!=nullptr)
						{self->m_vt.key_down(self->r_cb_obj,*self,scancode,keymask,self->m_id);}
					}
				break;
			}
		return FALSE;
		}
	if(scancode==15)
		{
		gtk_window_set_focus(GTK_WINDOW(widget),self->r_focus_old);
		return TRUE;
		}
	if(self->r_cb_obj!=nullptr)
		{self->m_vt.key_down(self->r_cb_obj,*self,scancode,keymask,self->m_id);}
	return TRUE;
	}

gboolean Window::Impl::key_up(GtkWidget* widget,GdkEvent* event,void* user_data)
	{
	auto w=gtk_window_get_focus(GTK_WINDOW(widget));
	if(w!=NULL)
		{return FALSE;}
	auto self=reinterpret_cast<Impl*>(user_data);
	auto& key=event->key;
	auto scancode=key.hardware_keycode - 8;
	auto keymask=keymaskFromSystem(key.state);
#ifndef __linux__
	#waring "Scancode key offset is not tested. Pressing esc should print 1"
	printf("%d\n",key.hardware_keycode - 8);
#endif
	if(self->r_cb_obj!=nullptr)
		{self->m_vt.key_up(self->r_cb_obj,*self,scancode,keymask,self->m_id);}
	return TRUE;
	}


gboolean Window::Impl::mouse_down(GtkWidget* widget,GdkEventButton* event,void* user_data)
	{
	auto self=reinterpret_cast<Impl*>(user_data);
	auto w=gtk_window_get_focus(GTK_WINDOW(widget));
	if(w!=NULL)
		{self->r_focus_old=w;}
	gtk_window_set_focus(GTK_WINDOW(widget),NULL);
	return FALSE;
	}

