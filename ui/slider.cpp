//@	{"targets":[{"name":"slider.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "slider.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class Slider::Impl:private Slider
	{
	public:
		Impl(Container& cnt,int id,bool vertical);
		~Impl();

		void callback(Callback cb,void* cb_obj)
			{
			m_cb=cb;
			m_cb_obj=cb_obj;
			}

		double value() const noexcept
			{return gtk_range_get_value(GTK_RANGE(m_handle));}

		void value(double x) noexcept
			{return gtk_range_set_value(GTK_RANGE(m_handle),x);}

		int id() const noexcept
			{return m_id;}

	private:
		int m_id;
		Callback m_cb;
		void* m_cb_obj;
		GtkScale* m_handle;
		static gboolean changed_callback(GtkWidget* widget,gpointer data);
	};

Slider::Slider(Container& cnt,int id,bool vertical)
	{m_impl=new Impl(cnt,id,vertical);}

Slider::~Slider()
	{delete m_impl;}

Slider& Slider::callback(Callback cb,void* cb_obj)
	{
	m_impl->callback(cb,cb_obj);
	return *this;
	}

double Slider::value() const noexcept
	{return m_impl->value();}

Slider& Slider::value(double x)
	{
	m_impl->value(x);
	return *this;
	}
int Slider::id() const noexcept
	{return m_impl->id();}



Slider::Impl::Impl(Container& cnt,int id,bool vertical):Slider(*this),m_id(id)
	,m_cb(nullptr)
	{
	auto widget=gtk_scale_new_with_range(vertical?
		GTK_ORIENTATION_VERTICAL:GTK_ORIENTATION_HORIZONTAL,0,1,1e-3);
	gtk_range_set_inverted(GTK_RANGE(widget),vertical); //GTK draws the slider upside down
	gtk_scale_set_draw_value(GTK_SCALE(widget),FALSE);
	g_signal_connect(widget,"value-changed",G_CALLBACK(changed_callback),this);
	m_handle=GTK_SCALE(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

Slider::Impl::~Impl()
	{
	m_impl=nullptr;
	m_cb=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

gboolean Slider::Impl::changed_callback(GtkWidget* widget,gpointer data)
	{
	auto state=reinterpret_cast<Impl*>(data);
	if(state->m_cb!=nullptr)
		{(state->m_cb)(state->m_cb_obj,*state);}
	return FALSE;
	}
