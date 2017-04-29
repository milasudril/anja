//@	{"targets":[{"name":"textentry.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "textentry.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class TextEntry::Impl:private TextEntry
	{
	public:
		Impl(Container& cnt,int id);
		~Impl();

		void callback(Callback cb,void* cb_obj)
			{
			m_cb=cb;
			m_cb_obj=cb_obj;
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

	private:
		int m_id;
		Callback m_cb;
		void* m_cb_obj;
		GtkEntry* m_handle;

		static gboolean focus_callback(GtkWidget* widget,GdkEvent* event,gpointer data);
	};

TextEntry::TextEntry(Container& cnt,int id)
	{m_impl=new Impl(cnt,id);}

TextEntry::~TextEntry()
	{delete m_impl;}

TextEntry& TextEntry::callback(Callback cb,void* cb_obj)
	{
	m_impl->callback(cb,cb_obj);
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



TextEntry::Impl::Impl(Container& cnt,int id):TextEntry(*this),m_id(id)
	,m_cb(nullptr)
	{
	printf("Entry %p ctor\n",this);

	auto widget=gtk_entry_new();
	g_signal_connect(widget,"focus-out-event",G_CALLBACK(focus_callback),this);
	m_handle=GTK_ENTRY(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

TextEntry::Impl::~Impl()
	{
	m_impl=nullptr;
	m_cb=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	printf("Entry %p dtor\n",this);
	}

gboolean TextEntry::Impl::focus_callback(GtkWidget* widget,GdkEvent* event,gpointer data)
	{
	auto state=reinterpret_cast<Impl*>(data);
	if(state->m_cb!=nullptr)
		{(state->m_cb)(state->m_cb_obj,*state);}
	return FALSE;
	}
