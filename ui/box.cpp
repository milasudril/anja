//@	{
//@  "targets":[{"name":"box.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "box.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class Box::Impl:private Box
	{
	public:
		Impl(Container& cnt,bool vertical,int global_spacing);
		~Impl();

		void _add(GtkWidget* handle) noexcept
			{
			gtk_box_pack_start(m_handle,handle,m_mode.flags&EXPAND,m_mode.flags&FILL
				,m_mode.padding);
			}

		void _show() noexcept
			{gtk_widget_show_all(GTK_WIDGET(m_handle));}

		void _sensitive(bool val)
			{gtk_widget_set_sensitive(GTK_WIDGET(m_handle),val);}

		void* _toplevel() const
			{return gtk_widget_get_toplevel(GTK_WIDGET(m_handle));}

		void homogenous(bool status) noexcept
			{gtk_box_set_homogeneous(m_handle,status);}


		void insertMode(const InsertMode& mode) noexcept
			{m_mode=mode;}

		void alignment(float x) noexcept;

		Rectangle boundingBox() const noexcept
			{
			auto widget=GTK_WIDGET(m_handle);
			auto size=Vec2<double>
				{
				 static_cast<double>( gtk_widget_get_allocated_width(widget) )
				,static_cast<double>( gtk_widget_get_allocated_height(widget) )
				};
			int xi;
			int yi;
			gtk_widget_translate_coordinates(widget,gtk_widget_get_toplevel(widget)
				,0,0,&xi,&yi);
			auto min=Vec2<double>{static_cast<double>(xi),static_cast<double>(yi)};


			return Rectangle{min,min + size};
			}

	private:
		static void destroy_callback (GtkWidget* object,gpointer user_data);
		GtkBox* m_handle;
		InsertMode m_mode;
	};

Box::Box(Container& cnt,bool vertical,int global_spacing)
	{m_impl=new Box::Impl(cnt,vertical,global_spacing);}

Box::~Box()
	{delete m_impl;}

Box& Box::add(void* handle)
	{
	m_impl->_add(GTK_WIDGET(handle));
	return *this;
	}

Box& Box::show()
	{
	m_impl->_show();
	return *this;
	}

Box& Box::sensitive(bool val)
	{
	m_impl->_sensitive(val);
	return *this;
	}

void* Box::toplevel() const
	{return m_impl->_toplevel();}

Box& Box::homogenous(bool status) noexcept
	{
	m_impl->homogenous(status);
	return *this;
	}

Box& Box::insertMode(const InsertMode& mode) noexcept
	{
	m_impl->insertMode(mode);
	return *this;
	}

Box& Box::alignment(float x) noexcept
	{
	m_impl->alignment(x);
	return *this;
	}

Rectangle Box::boundingBox() const noexcept
	{return m_impl->boundingBox();}


Box::Impl::Impl(Container& cnt,bool vertical,int global_spacing):Box(*this),m_mode{0,0}
	{
	auto widget=gtk_box_new(vertical?GTK_ORIENTATION_VERTICAL:GTK_ORIENTATION_HORIZONTAL,global_spacing);
	cnt.add(widget);
	g_object_ref_sink(widget);
	m_handle=GTK_BOX(widget);
	}

Box::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

void Box::Impl::alignment(float x) noexcept
	{
	if(x<1.0f/3.0f)
		{gtk_box_set_baseline_position(m_handle,GTK_BASELINE_POSITION_TOP);}
	else
	if(x>2.0f/3.0f)
		{gtk_box_set_baseline_position(m_handle,GTK_BASELINE_POSITION_BOTTOM);}
	else
		{{gtk_box_set_baseline_position(m_handle,GTK_BASELINE_POSITION_CENTER);}}
	}
