//@	{"targets":[{"name":"slider.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "slider.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class Slider::Impl:private Slider
	{
	public:
		Impl(Container& cnt,bool vertical);
		~Impl();

		void callback(Callback cb,void* cb_obj,int id) noexcept
			{
			r_cb=cb;
			r_cb_obj=cb_obj;
			m_id=id;
			}

		double value() const noexcept
			{return gtk_range_get_value(GTK_RANGE(m_handle));}

		void value(double x) noexcept
			{return gtk_range_set_value(GTK_RANGE(m_handle),x);}

		int id() const noexcept
			{return m_id;}

	private:
		int m_id;
		Callback r_cb;
		void* r_cb_obj;
		GtkScale* m_handle;
		static gboolean changed_callback(GtkWidget* widget,gpointer data);
	};

Slider::Slider(Container& cnt,bool vertical)
	{m_impl=new Impl(cnt,vertical);}

Slider::~Slider()
	{delete m_impl;}

Slider& Slider::callback(Callback cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
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



Slider::Impl::Impl(Container& cnt,bool vertical):Slider(*this),m_id(0)
	,r_cb(nullptr)
	{
	auto widget=gtk_scale_new_with_range(vertical?
		GTK_ORIENTATION_VERTICAL:GTK_ORIENTATION_HORIZONTAL,0,1,1e-3);
	gtk_range_set_inverted(GTK_RANGE(widget),vertical); //GTK draws the slider upside down
	gtk_scale_set_draw_value(GTK_SCALE(widget),FALSE);
	gtk_widget_set_size_request(widget,vertical?32:96,vertical?96:32);

	GtkRequisition minsize;
	GtkRequisition natsize;
	gtk_widget_get_preferred_size(widget,&minsize,&natsize);

	g_signal_connect(widget,"value-changed",G_CALLBACK(changed_callback),this);
	m_handle=GTK_SCALE(widget);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

Slider::Impl::~Impl()
	{
	m_impl=nullptr;
	r_cb=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

gboolean Slider::Impl::changed_callback(GtkWidget* widget,gpointer data)
	{
	auto state=reinterpret_cast<Impl*>(data);
	if(state->r_cb!=nullptr)
		{state->r_cb(state->r_cb_obj,*state);}
	return FALSE;
	}
