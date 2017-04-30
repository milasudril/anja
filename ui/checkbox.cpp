//@	{"targets":[{"name":"checkbox.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "checkbox.hpp"
#include "container.hpp"

#include <gtk/gtk.h>

using namespace Anja;

class Checkbox::Impl:public Checkbox
	{
	public:
		Impl(Container& cnt,int id,const char* title);
		~Impl();

		bool state() const noexcept
			{return gtk_toggle_button_get_active(m_handle);}

		void state(bool state_new)
			{gtk_toggle_button_set_active(m_handle,state_new);}

		int id() const noexcept
			{return m_id;}

		void callback(Callback cb,void* cb_obj)
			{
			r_cb=cb;
			r_cb_obj=cb_obj;
			}

	private:
		int m_id;
		Callback r_cb;
		void* r_cb_obj;
		GtkToggleButton* m_handle;

		static void clicked_callback(GtkWidget* widget,gpointer data);
	};


Checkbox::Checkbox(Container& cnt,int id,const char* title)
	{
	m_impl=new Impl(cnt,id,title);
	}

Checkbox::~Checkbox()
	{
	delete m_impl;
	}


bool Checkbox::state() const noexcept
	{return m_impl->state();}

Checkbox& Checkbox::state(bool state_new)
	{
	m_impl->state(state_new);
	return *this;
	}

int Checkbox::id() const noexcept
	{return m_impl->id();}

Checkbox& Checkbox::callback(Callback cb,void* cb_obj)
	{
	m_impl->callback(cb,cb_obj);
	return *this;
	}



void Checkbox::Impl::clicked_callback(GtkWidget* widget,gpointer data)
	{
	auto self=reinterpret_cast<Impl*>(data);
	if(self->r_cb)
		{self->r_cb(self->r_cb_obj,*self);}
	}

Checkbox::Impl::Impl(Container& cnt,int id,const char* title):Checkbox(*this),m_id(id),r_cb(nullptr)
	{
	GtkWidget* widget=gtk_check_button_new_with_label(title);
	g_signal_connect(widget,"clicked",G_CALLBACK(clicked_callback),this);
	m_handle=GTK_TOGGLE_BUTTON(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

Checkbox::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

