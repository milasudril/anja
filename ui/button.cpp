//@	{"targets":[{"name":"button.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "button.hpp"
#include "container.hpp"
#include "focussink.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class Button::Impl:private Button
	{
	public:
		Impl(Container& cnt,const char* label);
		~Impl();

		void callback(Callback cb,void* cb_obj,int id)
			{
			r_cb=cb;
			r_cb_obj=cb_obj;
			m_id=id;
			}

		const char* label() const noexcept
			{return gtk_button_get_label(GTK_BUTTON(m_handle));}

		void label(const char* text) noexcept
			{return gtk_button_set_label(GTK_BUTTON(m_handle),text);}

		int id() const noexcept
			{return m_id;}

		void state(bool s) noexcept
			{
			auto cb=r_cb;
			r_cb=nullptr;
			gtk_toggle_button_set_active(m_handle,s);
			r_cb=cb;
			}

		bool state() const noexcept
			{return gtk_toggle_button_get_active(m_handle);}

		void focus() noexcept
			{gtk_widget_grab_focus(GTK_WIDGET(m_handle));}

	private:
		int m_id;
		Callback r_cb;
		void* r_cb_obj;
		GtkToggleButton* m_handle;

		static void clicked_callback(GtkWidget* widget,gpointer data);
		static gboolean focus_in_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data);
	};

Button::Button(Container& cnt,const char* label)
	{m_impl=new Impl(cnt,label);}

Button::~Button()
	{delete m_impl;}

Button& Button::callback(Callback cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

const char* Button::label() const noexcept
	{return m_impl->label();}

Button& Button::label(const char* text)
	{
	m_impl->label(text);
	return *this;
	}

int Button::id() const noexcept
	{return m_impl->id();}

Button& Button::state(bool s) noexcept
	{
	m_impl->state(s);
	return *this;
	}

Button& Button::focus() noexcept
	{
	m_impl->focus();
	return *this;
	}


bool Button::state() const noexcept
	{
	return m_impl->state();
	}



Button::Impl::Impl(Container& cnt,const char* lab):Button(*this),m_id(0)
	,r_cb(nullptr)
	{
	auto widget=gtk_toggle_button_new();
	g_signal_connect(widget,"clicked",G_CALLBACK(clicked_callback),this);
	g_signal_connect(widget,"focus-in-event",G_CALLBACK(focus_in_callback),this);
	m_handle=GTK_TOGGLE_BUTTON(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	label(lab);
	}

Button::Impl::~Impl()
	{
	m_impl=nullptr;
	r_cb=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	g_object_unref(m_handle);
	}

void Button::Impl::clicked_callback(GtkWidget* widget,gpointer data)
	{
	auto state=reinterpret_cast<Impl*>(data);
	if(state->r_cb!=nullptr)
		{state->r_cb(state->r_cb_obj,*state);}
	}

gboolean Button::Impl::focus_in_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto root=gtk_widget_get_toplevel(widget);
	auto sink=reinterpret_cast<const FocusSink*>( g_object_get_data(G_OBJECT(root),"anja-focus-sink") );
	if(sink!=nullptr)
		{sink->action(sink->object);}
	return TRUE;
	}
