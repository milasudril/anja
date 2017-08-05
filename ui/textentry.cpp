//@	{"targets":[{"name":"textentry.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "textentry.hpp"
#include "container.hpp"
#include "focussink.hpp"
#include <gtk/gtk.h>

using namespace Anja;

static GtkCssProvider* s_smallstyle=nullptr;
static size_t s_style_refcount=0;

class TextEntry::Impl:private TextEntry
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void callback(Callback cb,void* cb_obj,int id)
			{
			r_cb=cb;
			r_cb_obj=cb_obj;
			m_id=id;
			}

		const char* content() const noexcept
			{return gtk_entry_get_text(m_handle);}

		void content(const char* text) noexcept
			{return gtk_entry_set_text(m_handle,text);}

		void width(int n) noexcept
			{
			gtk_entry_set_width_chars(m_handle,n);
			gtk_entry_set_max_width_chars(m_handle,n);
			}

		void small(bool status)
			{
			auto context=gtk_widget_get_style_context(GTK_WIDGET(m_handle));
			if(status)
				{
				gtk_style_context_add_provider(context,GTK_STYLE_PROVIDER(s_smallstyle),
					GTK_STYLE_PROVIDER_PRIORITY_APPLICATION+50);
				}
			else
				{gtk_style_context_remove_provider(context,GTK_STYLE_PROVIDER(s_smallstyle));}
			}

		void alignment(float x)
			{gtk_entry_set_alignment(m_handle,x);}

		int id() const noexcept
			{return m_id;}

		void focus()
			{gtk_widget_grab_focus(GTK_WIDGET(m_handle));}

	private:
		int m_id;
		Callback r_cb;
		void* r_cb_obj;
		GtkEntry* m_handle;

		static gboolean focus_callback(GtkWidget* widget,GdkEvent* event,gpointer data);
		static gboolean focus_in_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data);
	};

TextEntry::TextEntry(Container& cnt)
	{m_impl=new Impl(cnt);}

TextEntry::~TextEntry()
	{delete m_impl;}

TextEntry& TextEntry::callback(Callback cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

const char* TextEntry::content() const noexcept
	{return m_impl->content();}

TextEntry& TextEntry::content(const char* text)
	{
	m_impl->content(text);
	return *this;
	}

TextEntry& TextEntry::width(int n) noexcept
	{
	m_impl->width(n);
	return *this;
	}

TextEntry& TextEntry::small(bool status) noexcept
	{
	m_impl->small(status);
	return *this;
	}

TextEntry& TextEntry::alignment(float x) noexcept
	{
	m_impl->alignment(x);
	return *this;
	}

int TextEntry::id() const noexcept
	{return m_impl->id();}

TextEntry& TextEntry::focus()
	{
	m_impl->focus();
	return *this;
	}



TextEntry::Impl::Impl(Container& cnt):TextEntry(*this),m_id(0)
	,r_cb(nullptr)
	{
	auto widget=gtk_entry_new();
	g_signal_connect(widget,"focus-in-event",G_CALLBACK(focus_in_callback),this);
	g_signal_connect(widget,"focus-out-event",G_CALLBACK(focus_callback),this);
	m_handle=GTK_ENTRY(widget);

//	g_object_ref_sink(widget);

	if(s_style_refcount==0)
		{
		s_smallstyle=gtk_css_provider_new();
		gtk_css_provider_load_from_data(s_smallstyle,"*{font-size:0.8em;padding:1px;min-height:0px}",-1,NULL);
		}
	++s_style_refcount;

	cnt.add(widget);
	}

TextEntry::Impl::~Impl()
	{
	m_impl=nullptr;
	r_cb=nullptr;
//Cleanup by resetting callbacks. GTK+ 3.22 screws up the heap if we call gtk_widget_destroy here
    g_signal_handlers_disconnect_by_func(m_handle,reinterpret_cast<void*>( focus_callback ),this);
    g_signal_handlers_disconnect_by_func(m_handle,reinterpret_cast<void*>( focus_in_callback ),this);
	if(s_style_refcount!=0)
		{
		auto context=gtk_widget_get_style_context(GTK_WIDGET(m_handle));
		gtk_style_context_remove_provider(context,GTK_STYLE_PROVIDER(s_smallstyle));
		}
	else
		{
		--s_style_refcount;
		if(s_style_refcount==0)
			{g_object_unref(s_smallstyle);}
		}
	}

gboolean TextEntry::Impl::focus_callback(GtkWidget* widget,GdkEvent* event,gpointer data)
	{
	//We must defer the event signal until next pass, otherwise GTK widgets break
	g_idle_add([](void* impl)
		{
		auto self=reinterpret_cast<Impl*>(impl);
		if(self->r_cb!=nullptr)
			{self->r_cb(self->r_cb_obj,*self);}
		return G_SOURCE_REMOVE;
		},data);
	return FALSE;
	}

gboolean TextEntry::Impl::focus_in_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto root=gtk_widget_get_toplevel(widget);
	auto sink=reinterpret_cast<const FocusSink*>( g_object_get_data(G_OBJECT(root),"anja-focus-sink") );
	if(sink!=nullptr)
		{sink->action(sink->object);}
	return TRUE;
	}
