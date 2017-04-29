//@	{"targets":[{"name":"label.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "label.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class Label::Impl:private Label
	{
	public:
		Impl(Container& cnt,const char* text);
		~Impl();

		const char* content() const noexcept
			{return gtk_label_get_text(m_handle);}

		void content(const char* text)
			{gtk_label_set_text(m_handle,text);}

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

Label::Impl::Impl(Container& cnt,const char* text):Label(*this)
	{
	printf("Label %p ctor\n",this);

	auto widget=gtk_label_new(text);
	
	m_handle=GTK_LABEL(widget);
	g_object_ref_sink(m_handle);
	cnt.add(widget);
	}

Label::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	printf("Label %p dtor\n",this);
	}