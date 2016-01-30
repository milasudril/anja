#ifdef __WAND__
target
	[
	name[keyboardview.o]
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

#include "keyboardview.h"
#include "keyboardlayout.h"
#include "guicontainer.h"
#include "guihandle.h"
#include "colorsystem.h"
#include "exceptionswallow.h"
#include <gtk/gtk.h>

#include <cmath>
#include <algorithm>

class KeyboardViewGtk:public KeyboardView
	{
	public:
		KeyboardViewGtk(GuiContainer& parent,KeyboardLayout& keyboard
			,EventHandler& handler);

		~KeyboardViewGtk();

		void update()
			{gtk_widget_queue_draw(m_canvas);}

		void eventHandlerSet(EventHandler& handler)
			{r_handler=&handler;}

		void keyboardLayoutSet(KeyboardLayout& keyboard)
			{
			r_keyboard=&keyboard;
			update();
			}

		const KeyboardLayout& keyboardLayoutGet() const
			{return *r_keyboard;}

		KeyboardLayout& keyboardLayoutGet()
			{return *r_keyboard;}

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_frame;}

	private:
		KeyboardLayout* r_keyboard;
		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_frame;
		GtkWidget* m_canvas;

		uint8_t scancodeGet(GtkWidget* object,float x,float y);

		static gboolean onMouseMove(GtkWidget* object,GdkEventMotion* event
			,void* keyboardviewgtk);

		static gboolean onMouseDown(GtkWidget* object,GdkEventButton* event
			,void* keyboardviewgtk);

		static gboolean onMouseUp(GtkWidget* object,GdkEventButton* event
			,void* keyboardviewgtk);

		static gboolean onKeyDown(GtkWidget *widget, GdkEventKey *event
			,void* keyboardviewgtk);

		static gboolean onKeyUp(GtkWidget *widget, GdkEventKey *event
			,void* keyboardviewgtk);

		static float keyDraw(cairo_t* cr, const KeyboardLayout::KeyDescriptor& key
			,double width,double x,double y,unsigned int row);

		static gboolean onPaint(GtkWidget* object,cairo_t* cr
			,void* keyboardviewgtk);
	};

KeyboardView::EventHandler KeyboardView::s_default_handler;

KeyboardView* KeyboardView::create
	(GuiContainer& parent,KeyboardLayout& keyboard,EventHandler& handler)
	{return new KeyboardViewGtk(parent,keyboard,handler);}

KeyboardViewGtk::KeyboardViewGtk
	(GuiContainer& parent,KeyboardLayout& keyboard,EventHandler& handler):
	r_keyboard(&keyboard),r_parent(parent),r_handler(&handler)
	{
	GtkWidget* frame=gtk_aspect_frame_new(NULL,0.5f,0.5f,15.0f/6.5f,TRUE);
	m_frame=frame;
	g_object_ref_sink(frame);
	gtk_frame_set_shadow_type ((GtkFrame*)frame,GTK_SHADOW_NONE);
	gtk_frame_set_label_align ((GtkFrame*)frame,0.0f,1.0f);
	gtk_widget_show(frame);
	parent.componentAdd(*this);

	m_canvas=gtk_drawing_area_new();
	gtk_widget_set_can_focus(m_canvas,TRUE);
	gtk_widget_add_events(m_canvas
		,GDK_POINTER_MOTION_MASK|GDK_BUTTON_PRESS_MASK
		|GDK_BUTTON_RELEASE_MASK|GDK_KEY_PRESS_MASK
		|GDK_KEY_RELEASE_MASK);

	g_signal_connect(m_canvas,"motion-notify-event",G_CALLBACK(onMouseMove),this);
	g_signal_connect(m_canvas,"button-press-event",G_CALLBACK(onMouseDown),this);
	g_signal_connect(m_canvas,"button-release-event",G_CALLBACK(onMouseUp),this);
	g_signal_connect(m_canvas,"key_press_event",G_CALLBACK(onKeyDown),this);
	g_signal_connect(m_canvas,"key_release_event",G_CALLBACK(onKeyUp),this);
	g_signal_connect(m_canvas,"draw",G_CALLBACK(onPaint),this);
	g_object_ref(m_canvas);
	gtk_container_add((GtkContainer*)frame,m_canvas);
	gtk_widget_show(m_canvas);
	gtk_widget_set_size_request(m_canvas,15*32,6.5*32);
	}

void KeyboardViewGtk::destroy()
	{delete this;}

KeyboardViewGtk::~KeyboardViewGtk()
	{
	gtk_widget_destroy(m_canvas);
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_frame);
	}

uint8_t KeyboardViewGtk::scancodeGet(GtkWidget* object,float x,float y)
	{
	auto width = gtk_widget_get_allocated_width (object);
	auto n_cols=r_keyboard->typingAreaCols();

	auto key_width=double(width)/(n_cols);
	x/=key_width;
	y/=key_width;

	auto scancode=r_keyboard->scancodeFXFromCoordinates(x-2.0f,y);

	return scancode==0?
		 r_keyboard->scancodeTAFromCoordinates(x,y-1.5f)
		:scancode;
	}

gboolean KeyboardViewGtk::onMouseMove(GtkWidget* object,GdkEventMotion* event
	,void* keyboardviewgtk)
	{
	KeyboardViewGtk* _this=reinterpret_cast<KeyboardViewGtk*>(keyboardviewgtk);
	auto scancode=_this->scancodeGet(object, event->x, event->y);
	EXCEPTION_SWALLOW(
		_this->r_handler->onMouseMove(*_this,scancode,keymaskFromSystem(event->state));
		,_this);
	return TRUE;
	}

gboolean KeyboardViewGtk::onMouseDown(GtkWidget* object,GdkEventButton* event
	,void* keyboardviewgtk)
	{
	gtk_widget_grab_focus(object);
	KeyboardViewGtk* _this=reinterpret_cast<KeyboardViewGtk*>(keyboardviewgtk);
	auto scancode=_this->scancodeGet(object, event->x, event->y);
	EXCEPTION_SWALLOW(
		_this->r_handler->onMouseDown(*_this,scancode,keymaskFromSystem(event->state));
		,_this);
	return TRUE;
	}

gboolean KeyboardViewGtk::onMouseUp(GtkWidget* object,GdkEventButton* event
	,void* keyboardviewgtk)
	{
	KeyboardViewGtk* _this=reinterpret_cast<KeyboardViewGtk*>(keyboardviewgtk);
	auto scancode=_this->scancodeGet(object,event->x, event->y);
	EXCEPTION_SWALLOW(
		_this->r_handler->onMouseUp(*_this,scancode,keymaskFromSystem(event->state));
		,_this);
	return TRUE;
	}

gboolean KeyboardViewGtk::onKeyDown(GtkWidget *widget, GdkEventKey *event
	,void* keyboardviewgtk)
	{
	KeyboardViewGtk* _this=reinterpret_cast<KeyboardViewGtk*>(keyboardviewgtk);
	EXCEPTION_SWALLOW(_this->r_handler->onKeyDown(*_this,event->hardware_keycode-8);
		,_this);
	return TRUE;
	}

gboolean KeyboardViewGtk::onKeyUp(GtkWidget *widget, GdkEventKey *event
	,void* keyboardviewgtk)
	{
	KeyboardViewGtk* _this=reinterpret_cast<KeyboardViewGtk*>(keyboardviewgtk);
	EXCEPTION_SWALLOW(_this->r_handler->onKeyUp(*_this,event->hardware_keycode-8);
		,_this);
	return TRUE;
	}

float KeyboardViewGtk::keyDraw(cairo_t* cr
	,const KeyboardLayout::KeyDescriptor& key,double width
	,double x,double y,unsigned int row)
	{
	size_t vertex=0;
	auto v=key.vertexGet(vertex);
	auto x_max=v.x;
	if(v.x==-1.0f)
		{return 0.0f;}

	auto color=key.colorGet();
	auto color_border=key.colorBorderGet();
	ColorSystem color_gtk{color};
	gdk_cairo_set_source_rgba(cr, color_gtk);
	cairo_move_to(cr,width*(v.x+x),width*(v.y+y+row));
	++vertex;
	while(v.x!=-1.0f)
		{
		v=key.vertexGet(vertex);
		if(v.x==-1.0f)
			{break;}
		cairo_line_to(cr,(v.x+x)*width,(v.y+y+row)*width);

		if(v.x>x_max)
			{x_max=v.x;}
		++vertex;
		}

	cairo_close_path(cr);
	cairo_fill_preserve (cr);
	cairo_set_line_width(cr,width/16);
	color_gtk=color_border;
	gdk_cairo_set_source_rgba(cr,color_gtk);
	cairo_stroke(cr);

	if(isLight(color))
		{color_gtk=COLORS[ColorID::BLACK];}
	else
		{color_gtk=COLORS[ColorID::WHITE];}
	gdk_cairo_set_source_rgba(cr, color_gtk);

	cairo_text_extents_t extents;
	cairo_text_extents(cr,key.labelGet(),&extents);
	cairo_move_to(cr,(x + 0.5*x_max)*width-0.5*extents.width
		,(y + row + 0.5)*width+0.5*extents.height);
	cairo_show_text(cr,key.labelGet());
	return x_max;
	}

gboolean KeyboardViewGtk::onPaint(GtkWidget* object,cairo_t* cr
	,void* keyboardviewgtk)
	{
//	This function needs some clean-up
	KeyboardViewGtk* _this=reinterpret_cast<KeyboardViewGtk*>(keyboardviewgtk);

	auto width = gtk_widget_get_allocated_width (object);
	auto n_cols=_this->r_keyboard->typingAreaCols();
	auto key_width=double(width)/(n_cols);

	cairo_set_font_size(cr,std::min(key_width/4,12.0));

	//	Draw function keys
		{
		auto keys=_this->r_keyboard->functionKeysBegin();
		auto keys_end=_this->r_keyboard->functionKeysEnd();
		double x=2;
		while(keys!=keys_end)
			{
			auto res=keyDraw(cr,*keys,key_width,x,0.0,0);
			x+=(res==0.0f)? 1.0f:res;
			++keys;
			}
		}

	//	Draw typing area
		{
		auto keys=_this->r_keyboard->typingAreaBegin();
		auto keys_end=_this->r_keyboard->typingAreaEnd();
		unsigned int k=0;
		unsigned int l=0;
		double x=0;
		while(keys!=keys_end)
			{
			x+=keyDraw(cr,*keys,key_width,x,1.5,l);
			++k;
			++keys;
			if(k%n_cols==0)
				{
				k=0;
				x=0;
				++l;
				}
			}
		}

	return FALSE;
	}
