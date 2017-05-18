//@	{
//@	"targets":[{"type":"object","name":"keyboardview.o","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "keyboardview.hpp"
#include "container.hpp"
#include "../common/arrayfixed.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class KeyboardView::Impl:public KeyboardView
	{
	public:
		Impl(Container& cnt);
		~Impl();

		int id() const noexcept
			{return m_id;}

		void callback(CallbackImpl cb,void* cb_obj,int id)
			{
			m_id=id;
			m_cb=cb;
			r_cb_obj=cb_obj;
			}


		int selection() const noexcept
			{return m_selection;}

		void selection(int scancode)
			{
			m_selection=scancode;
			gtk_widget_queue_draw(GTK_WIDGET(m_canvas));
			}

		const KeyLabel& keyLabel(int scancode) const noexcept
			{return m_labels[scancode];}

		void keyLabel(int scancode,const KeyLabel& lbl)
			{m_labels[scancode]=lbl;}

		void redraw()
			{gtk_widget_queue_draw(GTK_WIDGET(m_canvas));}


	private:
		int m_id;
		CallbackImpl m_cb;
		void* r_cb_obj;
		int m_selection;
		GtkDrawingArea* m_canvas;

		ArrayFixed<KeyLabel,256> m_labels;

		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
	};



KeyboardView::KeyboardView(Container& cnt)
	{
	m_impl=new Impl(cnt);
	}

KeyboardView::~KeyboardView()
	{
	delete m_impl;
	}

int KeyboardView::id() const noexcept
	{return m_impl->id();}

KeyboardView& KeyboardView::callback(CallbackImpl cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

int KeyboardView::selection() const noexcept
	{return m_impl->selection();}

KeyboardView& KeyboardView::selection(int scancode)
	{
	m_impl->selection(scancode);
	return *this;
	}

const KeyboardView::KeyLabel& KeyboardView::keyLabel(int scancode) const noexcept
	{return m_impl->keyLabel(scancode);}

KeyboardView& KeyboardView::keyLabel(int scancode,const KeyLabel& label)
	{
	m_impl->keyLabel(scancode,label);
	return *this;
	}

KeyboardView& KeyboardView::redraw()
	{
	m_impl->redraw();
	return *this;
	}



KeyboardView::Impl::Impl(Container& cnt):KeyboardView(*this)
	{
	auto widget=gtk_drawing_area_new();
	m_canvas=GTK_DRAWING_AREA(widget);
	gtk_widget_add_events(widget,GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK);
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	g_signal_connect(widget,"button-release-event",G_CALLBACK(mouse_up),this);
	gtk_widget_set_size_request(widget,15*32,6*32);
	g_object_ref_sink(m_canvas);
	cnt.add(m_canvas);
	}

KeyboardView::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_canvas));
	}


gboolean KeyboardView::Impl::draw(GtkWidget* object,cairo_t* cr,void* obj)
	{
	return FALSE;
	}

gboolean KeyboardView::Impl::mouse_up(GtkWidget* object,GdkEventButton* event,void* obj)
	{
	return TRUE;
	}
