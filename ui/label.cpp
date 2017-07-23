//@	{"targets":[{"name":"label.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "label.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Anja;

static GtkCssProvider* s_smallstyle=nullptr;
static size_t s_style_refcount=0;

class Label::Impl:private Label
	{
	public:
		Impl(Container& cnt,const char* text);
		~Impl();

		const char* content() const noexcept
			{return gtk_label_get_text(m_handle);}

		void content(const char* text)
			{gtk_label_set_text(m_handle,text);}

		void wordwrap(bool status)
			{
			gtk_label_set_line_wrap(m_handle,status);
			if(status)
				{
				gtk_label_set_max_width_chars(m_handle,60);
				gtk_widget_set_size_request(GTK_WIDGET(m_handle),500,1);
				}
			else
				{
				gtk_label_set_max_width_chars(m_handle,-1);
				gtk_widget_set_size_request(GTK_WIDGET(m_handle),-1,-1);
				}
			}

		void small(bool status)
			{
			auto context=gtk_widget_get_style_context(GTK_WIDGET(m_handle));
			if(status)
				{
				gtk_style_context_add_provider(context,GTK_STYLE_PROVIDER(s_smallstyle),
					GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
				}
			else
				{gtk_style_context_remove_provider(context,GTK_STYLE_PROVIDER(s_smallstyle));}
			}


		void alignment(float x)
			{gtk_label_set_xalign(m_handle,x);}

	private:
		GtkLabel* m_handle;
	};

Label::Label(Container& cnt,const char* text)
	{m_impl=new Impl(cnt,text);}

Label::~Label()
	{delete m_impl;}

const char* Label::content() const noexcept
	{return m_impl->content();}

Label& Label::content(const char* x)
	{
	m_impl->content(x);
	return *this;
	}

Label& Label::wordwrap(bool status)
	{
	m_impl->wordwrap(status);
	return *this;
	}

Label& Label::small(bool status)
	{
	m_impl->small(status);
	return *this;
	}

Label& Label::alignment(float x)
	{
	m_impl->alignment(x);
	return *this;
	}


Label::Impl::Impl(Container& cnt,const char* text):Label(*this)
	{
	auto widget=gtk_label_new(text);

	m_handle=GTK_LABEL(widget);
	g_object_ref_sink(m_handle);
	cnt.add(widget);

	if(s_style_refcount==0)
		{
		s_smallstyle=gtk_css_provider_new();
		gtk_css_provider_load_from_data(s_smallstyle,"*{font-size:0.8em;padding:1px}",-1,NULL);
		}
	++s_style_refcount;
	}

Label::Impl::~Impl()
	{
	m_impl=nullptr;
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
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}
