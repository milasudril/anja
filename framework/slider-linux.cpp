#ifdef __WAND__
target
	[
	name[slider.o]
	type[object]
	platform[;GNU/Linux]
	dependency[gtk-3;external]
	dependency[gdk-3;external]
	dependency[pangocairo-1.0;external]
	dependency[pango-1.0;external]
	dependency[atk-1.0;external]
	dependency[cairo-gobject;external]
	dependency[cairo;external]
	dependency[gdk_pixbuf-2.0;external]
	dependency[gio-2.0;external]
	dependency[gobject-2.0;external]
	dependency[glib-2.0;external]
	]
#endif

#include "slider.h"
#include "guicontainer.h"
#include <gtk/gtk.h>

#include <cstdio>

class Slider::Impl
	{
	public:
		Impl(Slider& me,EventHandler& handler,double& value):r_handler(handler)
			,r_value(value)
			{
			printf("CTOR %p\n",this);
			m_box=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
			gtk_box_set_homogeneous((GtkBox*)m_box,FALSE);

			m_slider=gtk_scale_new_with_range(GTK_ORIENTATION_VERTICAL,0,1,1e-3);
			gtk_scale_set_has_origin((GtkScale*)m_slider,FALSE);
			gtk_scale_set_draw_value((GtkScale*)m_slider,FALSE);
			gtk_range_set_inverted((GtkRange*)m_slider,TRUE);
			gtk_box_pack_start((GtkBox*)m_box,m_slider,TRUE,TRUE,0);
			gtk_widget_show(m_slider);

			m_text=gtk_entry_new();
			gtk_box_pack_end((GtkBox*)m_box,m_text,FALSE,FALSE,0);
			gtk_widget_show(m_text);
			gtk_entry_set_width_chars((GtkEntry*)m_text,5);

			g_signal_connect(m_slider,"value-changed",G_CALLBACK(sliderMoved),this);
			g_signal_connect(m_text,"key_release_event",G_CALLBACK(textChanged),this);
			g_signal_connect(m_box,"destroy",G_CALLBACK(onDestroy),&me);
			update();
			}

		~Impl()
			{
			printf("DTOR %p\n",this);
			gtk_widget_destroy(m_text);
			gtk_widget_destroy(m_slider);
			gtk_widget_destroy(m_box);
			}

		void update()
			{
			gtk_range_set_value((GtkRange*)m_slider,r_value);
			}

		void addTo(GuiContainer& parent)
			{parent.objectAdd(m_box);}

	private:
		static void sliderMoved(GtkRange* range,void* impl)
			{
			Impl* _this=(Impl*)impl;
			double v=gtk_range_get_value(range);
			_this->r_value=v;
			gtk_entry_set_text((GtkEntry*)_this->m_text,_this->r_handler.textGet(v));
			}

		static gint textChanged(GtkWidget* entry,GdkEvent* event,void* impl)
			{
			Impl* _this=(Impl*)impl;
			GtkEntry* text=(GtkEntry*)entry;
			auto v=_this->r_handler.valueGet(gtk_entry_get_text(text));
			if(v>=0 || v<=1)
				{
				_this->r_value=v;
				_this->update();
				}
			return 1;
			}

		static void onDestroy(GtkWidget* object,void* user_data)
			{
			Slider* me=(Slider*)user_data;
			delete me;
			}

		EventHandler& r_handler;
		double& r_value;

		GtkWidget* m_box;
		GtkWidget* m_slider;
		GtkWidget* m_text;
	};

Slider::Slider(GuiContainer& parent,EventHandler& handler):m_value(0)
	{
	printf("CTOR %p\n",this);
	m_impl=new Impl(*this,handler,m_value);
	m_impl->addTo(parent);
	}

void Slider::valueSet(double value)
	{
	m_value=value;
	m_impl->update();
	}

Slider::~Slider()
	{
	delete m_impl;
	printf("DTOR %p\n",this);
	}
