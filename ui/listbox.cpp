//@	{"targets":[{"name":"listbox.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "listbox.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class Listbox::Impl:private Listbox
	{
	public:
		Impl(Container& cnt);
		~Impl();

		int id() const noexcept
			{return m_id;}

		void append(const char* option)
			{
			gtk_combo_box_text_append_text(m_handle,option);
			}

		void replace(int index,const char* option)
			{
			auto i=selected();
			auto cb=r_cb;
			r_cb=nullptr;
			gtk_combo_box_text_remove(m_handle,index);
			gtk_combo_box_text_insert(m_handle,index,NULL,option);
			selected(i);
			r_cb=cb;
			}

		void selected(int index) noexcept
			{
			auto cb=r_cb;
			r_cb=nullptr;
			gtk_combo_box_set_active(GTK_COMBO_BOX(m_handle),index);
			r_cb=cb;
			}

		int selected() const noexcept
			{return gtk_combo_box_get_active(GTK_COMBO_BOX(m_handle));}

		void clear() noexcept
			{
			auto tmp=r_cb;
			r_cb=nullptr;
			gtk_combo_box_text_remove_all(m_handle);
			r_cb=tmp;
			}

		void callback(Callback cb,void* cb_obj,int id)
			{
			r_cb=cb;
			r_cb_obj=cb_obj;
			m_id=id;
			}

	private:
		int m_id;
		Callback r_cb;
		void* r_cb_obj;
		GtkComboBoxText* m_handle;

		static void changed_callback(GtkComboBox* widget,void* listboxgtk)
			{
			auto self=reinterpret_cast<Impl*>(listboxgtk);
			if(self->r_cb!=nullptr)
				{self->r_cb(self->r_cb_obj,*self);}
			}
	};

Listbox::Listbox(Container& cnt)
	{m_impl=new Impl(cnt);}

Listbox::~Listbox()
	{delete m_impl;}

int Listbox::id() const noexcept
	{return m_impl->id();}

Listbox& Listbox::append(const char* option)
	{
	m_impl->append(option);
	return *this;
	}

Listbox& Listbox::replace(int index,const char* option)
	{
	m_impl->replace(index,option);
	return *this;
	}

Listbox& Listbox::selected(int index) noexcept
	{
	m_impl->selected(index);
	return *this;
	}

int Listbox::selected() const noexcept
	{return m_impl->selected();}

Listbox& Listbox::clear() noexcept
	{
	m_impl->clear();
	return *this;
	}

Listbox& Listbox::callback(Callback cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}


Listbox::Impl::Impl(Container& cnt):Listbox(*this),m_id(0),r_cb(nullptr)
	{
	auto widget=gtk_combo_box_text_new();
	m_handle=GTK_COMBO_BOX_TEXT(widget);
	g_signal_connect(widget,"changed",G_CALLBACK(changed_callback),this);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

Listbox::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}
