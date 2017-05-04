//@	{
//@  "targets":[{"name":"paned.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "paned.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class Paned::Impl:private Paned
	{
	public:
		Impl(Container& cnt,bool vertical);
		~Impl();

		void _add(GtkWidget* handle) noexcept
			{
			auto child=handle;
			switch(m_position)
				{
				case Index::FIRST:
					gtk_paned_pack1(m_handle,child
						,(m_mode.flags&RESIZE)==RESIZE
						,(m_mode.flags&SHRINK_ALLOWED)==SHRINK_ALLOWED);
					m_position=Index::SECOND;
					if(m_vertical)
						{gtk_widget_set_margin_bottom(handle,3);}
					else
						{gtk_widget_set_margin_end(handle,3);}
					break;

				case Index::SECOND:
					gtk_paned_pack2(m_handle,child
						,(m_mode.flags&RESIZE)==RESIZE
						,(m_mode.flags&SHRINK_ALLOWED)==SHRINK_ALLOWED);
					if(m_vertical)
						{gtk_widget_set_margin_top(handle,3);}
					else
						{gtk_widget_set_margin_start(handle,3);}
					break;
				}
			}

		void _show() noexcept 
			{gtk_widget_show_all(GTK_WIDGET(m_handle));}

		void _sensitive(bool val)
			{gtk_widget_set_sensitive(GTK_WIDGET(m_handle),val);}

		void* _toplevel() const
			{return gtk_widget_get_toplevel(GTK_WIDGET(m_handle));}

		void insertMode(const InsertMode& mode) noexcept
			{m_mode=mode;}

	private:
		GtkPaned* m_handle;
		InsertMode m_mode;
		bool m_vertical;

		enum class Index:unsigned short{FIRST,SECOND};
		Index m_position;
	};

Paned::Paned(Container& cnt,bool vertical)
	{
	printf("Paned %p ctor\n",this);
	m_impl=new Paned::Impl(cnt,vertical);
	}

Paned::~Paned()
	{
	delete m_impl;
	printf("Paned %p dtor\n",this);
	}

Paned& Paned::add(void* handle)
	{
	m_impl->_add(GTK_WIDGET(handle));
	return *this;
	}

Paned& Paned::show()
	{
	m_impl->_show();
	return *this;
	}

Paned& Paned::sensitive(bool val)
	{
	m_impl->_sensitive(val);
	return *this;
	}

void* Paned::toplevel() const
	{return m_impl->_toplevel();}

Paned& Paned::insertMode(const InsertMode& mode) noexcept
	{
	m_impl->insertMode(mode);
	return *this;
	}



Paned::Impl::Impl(Container& cnt,bool vertical):Paned(*this)
	,m_mode{0},m_vertical(vertical),m_position{Index::FIRST}
	{
	printf("Paned::Impl %p ctor\n",this);
	auto widget=gtk_paned_new(vertical?GTK_ORIENTATION_VERTICAL:GTK_ORIENTATION_HORIZONTAL);
	cnt.add(widget);
	g_object_ref_sink(widget);
	m_handle=GTK_PANED(widget);
	}

Paned::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	printf("Paned::Impl %p dtor\n",this);
	}
