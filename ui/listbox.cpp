//@	{"targets":[{"name":"listbox.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "listbox.hpp"
#include "container.hpp"
#include "focussink.hpp"
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
			gtk_list_box_insert(m_handle,gtk_label_new(option),-1);
			}

		void selected(int index,bool status) noexcept
			{
			auto cb=r_cb;
			r_cb=nullptr;
			auto row=gtk_list_box_get_row_at_index(m_handle,index);
			if(status)
				{gtk_list_box_select_row(m_handle,row);}
			else
				{gtk_list_box_unselect_row(m_handle,row);}
			r_cb=cb;
			}

		bool selected(int index) const noexcept
			{
			auto row=gtk_list_box_get_row_at_index(m_handle,index);
			return gtk_list_box_row_is_selected(row);
			}

		void clear() noexcept
			{
			auto tmp=r_cb;
			r_cb=nullptr;
		//	Fixme
		//	gtk_list_box_remove_all(m_handle);
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
		GtkListBox* m_handle;

		static void changed_callback(GtkComboBox* widget,void* listboxgtk)
			{
			auto self=reinterpret_cast<Impl*>(listboxgtk);
			if(self->r_cb!=nullptr)
				{self->r_cb(self->r_cb_obj,*self);}
			}


		static gboolean focus_in_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
			{
			auto root=gtk_widget_get_toplevel(widget);
			auto sink=reinterpret_cast<const FocusSink*>( g_object_get_data(G_OBJECT(root),"anja-focus-sink") );
			if(sink!=nullptr)
				{sink->action(sink->object);}
			return TRUE;
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

Listbox& Listbox::selected(int index,bool status) noexcept
	{
	m_impl->selected(index,status);
	return *this;
	}

bool Listbox::selected(int index) const noexcept
	{return m_impl->selected(index);}

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
	auto widget=gtk_list_box_new();
	m_handle=GTK_LIST_BOX(widget);
	g_signal_connect(widget,"selected-rows-changed",G_CALLBACK(changed_callback),this);
	g_signal_connect(widget,"focus-in-event",G_CALLBACK(focus_in_callback),this);
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

Listbox::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}
