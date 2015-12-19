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
#include "guihandle.h"
#include "array_fixed.h"
#include <gtk/gtk.h>

#include <cstdlib>
#include <cstdio>

class SliderGtk:public Slider
	{
	public:
		SliderGtk(GuiContainer& parent,EventHandler& handler,bool horizontal);
		~SliderGtk();

		void destroy();
		void valueSet(double value);

	private:
		static void sliderMoved(GtkRange* range,void* slidergtk);
		static gint textChanged(GtkWidget* entry,GdkEvent* event,void* slidergtk);
		static void onDestroy(GtkWidget* object,void* slidergtk);

		GuiContainer& r_parent;
		EventHandler& r_handler;

		double m_value;
		GtkWidget* m_box;
		GtkWidget* m_slider;
		GtkWidget* m_text;
	};

void Slider::EventHandler::textGet(double value,TextBuffer& buffer)
	{
	sprintf(buffer.begin(),"%.3f",value);
	}

double Slider::EventHandler::valueGet(const char* text)
	{
	char* result;
	double v=strtod(text,&result);
	if(result==text)
		{return -1;}
	return v;
	}

Slider::EventHandler Slider::s_default_handler;

Slider* Slider::create(GuiContainer& parent,Slider::EventHandler& handler
	,bool horizontal)
	{return new SliderGtk(parent,handler,horizontal);}

void SliderGtk::onDestroy(GtkWidget* object,void* slidergtk)
	{
	SliderGtk* _this=(SliderGtk*)slidergtk;
	delete _this;
	}

void SliderGtk::sliderMoved(GtkRange* range,void* slidergtk)
	{
	SliderGtk* _this=(SliderGtk*)slidergtk;
	double v=gtk_range_get_value(range);
	EventHandler::TextBuffer buffer;
	_this->r_handler.textGet(v,buffer);
	gtk_entry_set_text((GtkEntry*)_this->m_text,buffer.begin());
	_this->m_value=v;
	}

gint SliderGtk::textChanged(GtkWidget* entry,GdkEvent* event,void* slidergtk)
	{
	SliderGtk* _this=(SliderGtk*)slidergtk;
	GtkEntry* text=(GtkEntry*)entry;
	auto v=_this->r_handler.valueGet(gtk_entry_get_text(text));
	if(v>=0 && v<=1)
		{_this->valueSet(v);}
	return 1;
	}

SliderGtk::SliderGtk(GuiContainer& parent,EventHandler& handler,bool horizontal):
	r_parent(parent),r_handler(handler)
	{
	gboolean invert=horizontal?
		FALSE:1;
	auto orientation=horizontal?
		GTK_ORIENTATION_HORIZONTAL:GTK_ORIENTATION_VERTICAL;
	m_box=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_set_homogeneous((GtkBox*)m_box,FALSE);

	m_slider=gtk_scale_new_with_range(orientation,0,1,1e-3);
	gtk_scale_set_has_origin((GtkScale*)m_slider,FALSE);
	gtk_scale_set_draw_value((GtkScale*)m_slider,FALSE);
	gtk_range_set_inverted((GtkRange*)m_slider,invert);
	gtk_box_pack_start((GtkBox*)m_box,m_slider,TRUE,TRUE,0);
	gtk_widget_show(m_slider);

	m_text=gtk_entry_new();
	gtk_box_pack_end((GtkBox*)m_box,m_text,FALSE,FALSE,0);
	gtk_widget_show(m_text);
	gtk_entry_set_width_chars((GtkEntry*)m_text,5);

	g_signal_connect(m_slider,"value-changed",G_CALLBACK(sliderMoved),this);
	g_signal_connect(m_text,"key_release_event",G_CALLBACK(textChanged),this);
	g_signal_connect(m_box,"destroy",G_CALLBACK(onDestroy),this);
	parent.componentAdd(m_box);
	valueSet(0.5);
	}

void SliderGtk::destroy()
	{
	r_parent.componentRemove(m_box);
	}

void SliderGtk::valueSet(double value)
	{
	m_value=value;
	gtk_range_set_value((GtkRange*)m_slider,value);
	}

SliderGtk::~SliderGtk()
	{}
