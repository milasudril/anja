//@	{"targets":[{"name":"textentry.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "textentry.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Anja;

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
			{gtk_entry_set_has_frame(m_handle,!status);}

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
	g_signal_connect(widget,"focus-out-event",G_CALLBACK(focus_callback),this);
	m_handle=GTK_ENTRY(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

TextEntry::Impl::~Impl()
	{
	m_impl=nullptr;
	r_cb=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

gboolean TextEntry::Impl::focus_callback(GtkWidget* widget,GdkEvent* event,gpointer data)
	{
	auto state=reinterpret_cast<Impl*>(data);
	if(state->r_cb!=nullptr)
		{state->r_cb(state->r_cb_obj,*state);}
	return FALSE;
	}
