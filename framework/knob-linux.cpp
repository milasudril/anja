#ifdef __WAND__
target
	[
	name[knob.o]
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

#include "knob.h"
#include "guicontainer.h"
#include "guihandle.h"
#include "array_fixed.h"
#include "exceptionswallow.h"
#include "colorsystem.h"
#include <gtk/gtk.h>

class KnobGtk:public Knob
	{
	public:
		KnobGtk(GuiContainer& parent,EventHandler& handler,const char* title
			,unsigned int id);
		~KnobGtk();

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_box;}

		void valueSet(double x);

		unsigned int idGet() const
			{return m_id;}

	private:
		void update()
			{gtk_widget_queue_draw(m_knob);}

		static gboolean knobOnDraw(GtkWidget* widget,cairo_t* cr,void* knobgtk);
		static gint textChanged(GtkWidget* entry,GdkEvent* event,void* knobgtk);

		static void onSizeChange(GtkWidget* widget,GtkAllocation* allocation
			,void* knobgtk);

		static gboolean onMouseMove(GtkWidget* object,GdkEventMotion* event
			,void* knobgtk);

		static gboolean onMouseDown(GtkWidget* object,GdkEventButton* event
			,void* knobgtk);

		static gboolean onMouseUp(GtkWidget* object,GdkEventButton* event
			,void* knobgtk);

		static gboolean onKeyDown(GtkWidget* widget,GdkEventKey* event
			,void* knobgtk);

		static gboolean onKeyUp(GtkWidget *widget, GdkEventKey *event
			,void* knobgtk);

		GuiContainer& r_parent;
		EventHandler& r_handler;
		unsigned int m_id;
		GuiHandle m_box;
		GtkWidget* m_title;
		GtkWidget* m_knob;
		GtkWidget* m_text;

		double m_o_x;
		double m_o_y;
		double m_value;
		bool m_drag;
	};



Knob* Knob::create(GuiContainer& parent,Knob::EventHandler& handler
	,const char* title,unsigned int id)
	{return new KnobGtk(parent,handler,title,id);}

void KnobGtk::destroy()
	{delete this;}

gboolean KnobGtk::textChanged(GtkWidget* entry,GdkEvent* event,void* knobgtk)
	{
	KnobGtk* _this=(KnobGtk*)knobgtk;
	GtkEntry* text=(GtkEntry*)entry;
	double v=0;
	EXCEPTION_SWALLOW(v=_this->r_handler.valueGet(*_this,gtk_entry_get_text(text));
		,_this);
	v=_this->r_handler.valueMapInverse(*_this,v);
	if(v>=0 && v<=1)
		{
		_this->m_value=v;
		_this->update();
		}
	return 1;
	}

gboolean KnobGtk::knobOnDraw(GtkWidget* widget,cairo_t* cr,void* knobgtk)
	{
	KnobGtk* _this=reinterpret_cast<KnobGtk*>(knobgtk);
	int dark_is;
	g_object_get(gtk_settings_get_default()
		,"gtk-application-prefer-dark-theme",&dark_is,NULL);

	auto width=gtk_widget_get_allocated_width(widget);
	auto height=gtk_widget_get_allocated_height(widget);

	double O_x=0.5*width;
	double O_y=0.5*height;
	double R=0.5*std::min(width-2,height-2);

	auto tau=2.0*acos(-1.0);

	cairo_arc(cr,O_x,O_y,R,0.0,tau);
	ColorSystem color_gtk{COLORS[dark_is?ColorID::GRAY_DARK : ColorID::GRAY]};
	gdk_cairo_set_source_rgba(cr,color_gtk);
	cairo_fill_preserve(cr);
	color_gtk=COLORS[dark_is?ColorID::GRAY:ColorID::GRAY_DARK ];
	gdk_cairo_set_source_rgba(cr,color_gtk);
	cairo_stroke(cr);

	auto value=_this->m_value;
	auto theta=-( (1.0-value)*tau*(-0.375) + value*(0.375*tau) )+0.25*tau;
	auto r=0.125*R;
	auto d=R-2.125*r;
	auto o_x=d*cos(theta)+O_x;
	auto o_y=height-(d*sin(theta)+O_y);
	if(gtk_widget_has_focus(widget))
		{
		color_gtk=ColorRGBA{ColorHSLA::fromHueAndLuma(0.33f
			,dark_is?0.7f:0.4f)};
		gdk_cairo_set_source_rgba(cr,color_gtk);
		}
	cairo_arc(cr,o_x,o_y,r,0.0,tau);
	cairo_fill(cr);
	_this->m_o_x=O_x;
	_this->m_o_y=O_y;


	return FALSE;
	}

void KnobGtk::onSizeChange(GtkWidget* widget,GtkAllocation* allocation
	,void* knobgtk)
	{
	KnobGtk* _this=reinterpret_cast<KnobGtk*>(knobgtk);
	_this->m_o_x=0.5*allocation->width;
	_this->m_o_y=0.5*allocation->height;
	}

gboolean KnobGtk::onMouseMove(GtkWidget* object,GdkEventMotion* event
	,void* knobgtk)
	{
	KnobGtk* _this=reinterpret_cast<KnobGtk*>(knobgtk);
	if(_this->m_drag)
		{
		auto x=event->x-_this->m_o_x;
		auto y=-(event->y-_this->m_o_y);


		auto tau=acos(-1);
		auto theta=0.5*atan2(x,y);
		auto value=(theta + tau*0.375)/(0.75*tau);

		value=std::min(1.0,std::max(value,0.0));
		EventHandler::TextBuffer text;
		_this->r_handler.textGet(*_this
			,_this->r_handler.valueMap(*_this,value),text);
		gtk_entry_set_text((GtkEntry*)_this->m_text,text.begin());
		_this->m_value=value;
		_this->update();
		}

	return TRUE;
	}

gboolean KnobGtk::onMouseDown(GtkWidget* object,GdkEventButton* event
	,void* knobgtk)
	{
	KnobGtk* _this=reinterpret_cast<KnobGtk*>(knobgtk);
	_this->m_drag=1;
	gtk_widget_grab_focus(object);
	return TRUE;
	}

gboolean KnobGtk::onMouseUp(GtkWidget* object,GdkEventButton* event
	,void* knobgtk)
	{
	KnobGtk* _this=reinterpret_cast<KnobGtk*>(knobgtk);
	_this->m_drag=0;
	return TRUE;
	}

gboolean KnobGtk::onKeyDown(GtkWidget* widget,GdkEventKey* event,void* knobgtk)
	{
	KnobGtk* _this=reinterpret_cast<KnobGtk*>(knobgtk);
	auto key=event->hardware_keycode-8;
	switch(key)
		{
		case 103:
		case 72:
			{
			auto value=std::max(0.0,std::min(_this->m_value + 1e-3,1.0));
			EventHandler::TextBuffer text;
			_this->r_handler.textGet(*_this
				,_this->r_handler.valueMap(*_this,value),text);
			gtk_entry_set_text((GtkEntry*)_this->m_text,text.begin());
			_this->m_value=value;
			_this->update();
			}
			return TRUE;

		case 108:
		case 80:
			{
			auto value=std::max(0.0,std::min(_this->m_value - 1e-3,1.0));
			EventHandler::TextBuffer text;
			_this->r_handler.textGet(*_this
				,_this->r_handler.valueMap(*_this,value),text);
			gtk_entry_set_text((GtkEntry*)_this->m_text,text.begin());
			_this->m_value=value;
			_this->update();
			}
			return TRUE;

		case 104:
			{
			auto value=std::max(0.0,std::min(_this->m_value + 1e-2,1.0));
			EventHandler::TextBuffer text;
			_this->r_handler.textGet(*_this
				,_this->r_handler.valueMap(*_this,value),text);
			gtk_entry_set_text((GtkEntry*)_this->m_text,text.begin());
			_this->m_value=value;
			_this->update();
			}
			return TRUE;

		case 109:
			{
			auto value=std::max(0.0,std::min(_this->m_value - 1e-2,1.0));
			EventHandler::TextBuffer text;
			_this->r_handler.textGet(*_this
				,_this->r_handler.valueMap(*_this,value),text);
			gtk_entry_set_text((GtkEntry*)_this->m_text,text.begin());
			_this->m_value=value;
			_this->update();
			}
			return TRUE;
		}

	return FALSE;
	}

KnobGtk::KnobGtk(GuiContainer& parent,EventHandler& handler,const char* title
	,unsigned int id):
	r_parent(parent),r_handler(handler),m_id(id),m_value(0.0),m_drag(0)
	{
	m_box=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	g_object_ref_sink(m_box);
	GtkWidget* box=m_box;
	gtk_box_set_homogeneous((GtkBox*)box,FALSE);

	m_title=gtk_label_new(title);
	g_object_ref_sink(m_title);
	gtk_box_pack_start((GtkBox*)box,m_title,FALSE,FALSE,0);

	gtk_widget_show(m_title);

	m_knob=gtk_drawing_area_new();
	gtk_widget_set_size_request(m_knob,32,32);
	gtk_widget_add_events(m_knob
		,GDK_POINTER_MOTION_MASK
		|GDK_BUTTON_PRESS_MASK
		|GDK_BUTTON_RELEASE_MASK
		|GDK_KEY_PRESS_MASK);
	gtk_widget_set_can_focus(m_knob,TRUE);
	g_signal_connect(m_knob,"draw",G_CALLBACK(knobOnDraw),this);
	g_signal_connect(m_knob,"motion-notify-event",G_CALLBACK(onMouseMove),this);
	g_signal_connect(m_knob,"button-press-event",G_CALLBACK(onMouseDown),this);
	g_signal_connect(m_knob,"button-release-event",G_CALLBACK(onMouseUp),this);
	g_signal_connect(m_knob,"key_press_event",G_CALLBACK(onKeyDown),this);
	g_signal_connect(m_knob, "size-allocate", G_CALLBACK(onSizeChange),this);
	g_object_ref_sink(m_knob);
	gtk_box_pack_start((GtkBox*)box,m_knob,TRUE,TRUE,0);
	gtk_widget_show(m_knob);

	m_text=gtk_entry_new();
	g_object_ref_sink(m_text);
	gtk_box_pack_end((GtkBox*)box,m_text,FALSE,FALSE,0);
	gtk_widget_show(m_text);
	gtk_entry_set_width_chars((GtkEntry*)m_text,5);

	g_signal_connect(m_text,"focus-out-event",G_CALLBACK(textChanged),this);
	parent.componentAdd(*this);
	}

KnobGtk::~KnobGtk()
	{
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_text);
	gtk_widget_destroy(m_knob);
	gtk_widget_destroy(m_title);
	gtk_widget_destroy(m_box);
	}

void KnobGtk::valueSet(double x)
	{
	auto v=r_handler.valueMapInverse(*this,x);
	if(v>=0 && v<=1)
		{
		EventHandler::TextBuffer text;
		r_handler.textGet(*this,x,text);
		gtk_entry_set_text((GtkEntry*)m_text,text.begin());
		m_value=v;
		update();
		}
	}
