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
#include <gtk/gtk.h>

#include <cmath>
#include <cstdio>
#include <algorithm>

class KeyboardViewGtk:public KeyboardView
	{
	public:
		KeyboardViewGtk(GuiContainer& parent,const KeyboardLayout& keyboard
			,EventHandler& handler);

		~KeyboardViewGtk();

		void addTo(GuiContainer& parent)
			{parent.objectAdd(m_canvas);}

		void update()
			{gtk_widget_queue_draw(m_canvas);}

		void eventHandlerSet(EventHandler& handler)
			{r_handler=&handler;}

		void keyboardLayoutSet(const KeyboardLayout& keyboard)
			{r_keyboard=&keyboard;}

		void destroy()
			{delete this;}

	private:
		const KeyboardLayout* r_keyboard;
		EventHandler* r_handler;
		GtkWidget* m_canvas;

		static void onDestroy(GtkWidget* object,void* keyboardviewgtk);
		static void nullCallback(GtkWidget* object,void* keyboardviewgtk);

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

		static gboolean onPaint(GtkWidget* object,cairo_t* cr
			,void* keyboardviewgtk);
	};

KeyboardView::EventHandler KeyboardView::s_null_handler;




KeyboardView* KeyboardView::instanceCreate
	(GuiContainer& parent,const KeyboardLayout& keyboard,EventHandler& handler)
	{return new KeyboardViewGtk(parent,keyboard,handler);}

KeyboardViewGtk::KeyboardViewGtk
	(GuiContainer& parent,const KeyboardLayout& keyboard,EventHandler& handler):
	r_keyboard(&keyboard),r_handler(&handler)
	{
	m_canvas=gtk_drawing_area_new();
	gtk_widget_set_can_focus(m_canvas,TRUE);
	gtk_widget_add_events(m_canvas
		,GDK_POINTER_MOTION_MASK|GDK_BUTTON_PRESS_MASK
		|GDK_BUTTON_RELEASE_MASK|GDK_KEY_PRESS_MASK
		|GDK_KEY_RELEASE_MASK);

	g_signal_connect(m_canvas,"destroy",G_CALLBACK(onDestroy),this);
	g_signal_connect(m_canvas,"motion-notify-event",G_CALLBACK(onMouseMove),this);
	g_signal_connect(m_canvas,"button-press-event",G_CALLBACK(onMouseDown),this);
	g_signal_connect(m_canvas,"button-release-event",G_CALLBACK(onMouseUp),this);
	g_signal_connect(m_canvas,"key_press_event",G_CALLBACK(onKeyDown),this);
	g_signal_connect(m_canvas,"key_release_event",G_CALLBACK(onKeyUp),this);
	g_signal_connect(m_canvas,"draw",G_CALLBACK(onPaint),this);
	addTo(parent);
	}

KeyboardViewGtk::~KeyboardViewGtk()
	{
	g_signal_handlers_disconnect_by_func(m_canvas,(void*)onPaint,this);
	g_signal_handlers_disconnect_by_func(m_canvas,(void*)onKeyUp,this);
	g_signal_handlers_disconnect_by_func(m_canvas,(void*)onKeyDown,this);
	g_signal_handlers_disconnect_by_func(m_canvas,(void*)onMouseUp,this);
	g_signal_handlers_disconnect_by_func(m_canvas,(void*)onMouseDown,this);
	g_signal_handlers_disconnect_by_func(m_canvas,(void*)onMouseMove,this);
	g_signal_handlers_disconnect_by_func(m_canvas,(void*)onDestroy,this);
	gtk_widget_destroy(m_canvas);
	}

void KeyboardViewGtk::onDestroy(GtkWidget* object,void* keyboardviewgtk)
	{
	KeyboardViewGtk* _this=(KeyboardViewGtk*)keyboardviewgtk;
	_this->destroy();
	}

gboolean KeyboardViewGtk::onMouseMove(GtkWidget* object,GdkEventMotion* event
	,void* keyboardviewgtk)
	{
	KeyboardViewGtk* _this=(KeyboardViewGtk*)keyboardviewgtk;
	auto width = gtk_widget_get_allocated_width (object);
	auto n_cols=_this->r_keyboard->typingAreaCols();

	auto key_width=double(width)/(n_cols);

	auto scancode=_this->r_keyboard->scancodeFromCoordinates(event->x/key_width,event->y/key_width);
	_this->r_handler->mouseMove(scancode,keymaskFromSystem(event->state));
	return TRUE;
	}

gboolean KeyboardViewGtk::onMouseDown(GtkWidget* object,GdkEventButton* event
	,void* keyboardviewgtk)
	{
	gtk_widget_grab_focus(object);
	KeyboardViewGtk* _this=(KeyboardViewGtk*)keyboardviewgtk;
	auto width = gtk_widget_get_allocated_width (object);
	auto n_cols=_this->r_keyboard->typingAreaCols();

	auto key_width=double(width)/(n_cols);

	auto scancode=_this->r_keyboard->scancodeFromCoordinates(event->x/key_width,event->y/key_width);
	_this->r_handler->mouseDown(scancode,keymaskFromSystem(event->state));
	return TRUE;
	}

gboolean KeyboardViewGtk::onMouseUp(GtkWidget* object,GdkEventButton* event
	,void* keyboardviewgtk)
	{
	KeyboardViewGtk* _this=(KeyboardViewGtk*)keyboardviewgtk;
	auto width = gtk_widget_get_allocated_width (object);
	auto n_cols=_this->r_keyboard->typingAreaCols();

	auto key_width=double(width)/(n_cols);

	auto scancode=_this->r_keyboard->scancodeFromCoordinates(event->x/key_width,event->y/key_width);
	_this->r_handler->mouseUp(scancode,keymaskFromSystem(event->state));
	return TRUE;
	}

gboolean KeyboardViewGtk::onKeyDown(GtkWidget *widget, GdkEventKey *event
	,void* keyboardviewgtk)
	{
	KeyboardViewGtk* _this=(KeyboardViewGtk*)keyboardviewgtk;
	_this->r_handler->keyDown(event->hardware_keycode-8);
	return TRUE;
	}

gboolean KeyboardViewGtk::onKeyUp(GtkWidget *widget, GdkEventKey *event
	,void* keyboardviewgtk)
	{
	KeyboardViewGtk* _this=(KeyboardViewGtk*)keyboardviewgtk;
	_this->r_handler->keyUp(event->hardware_keycode-8);
	return TRUE;
	}

gboolean KeyboardViewGtk::onPaint(GtkWidget* object,cairo_t* cr
	,void* keyboardviewgtk)
	{
//	This function needs some clean-up
	KeyboardViewGtk* _this=(KeyboardViewGtk*)keyboardviewgtk;

	auto width = gtk_widget_get_allocated_width (object);
	auto n_cols=_this->r_keyboard->typingAreaCols();

	auto key_width=double(width)/(n_cols);

	cairo_set_font_size(cr,std::min(key_width/4,12.0));

	auto keys=_this->r_keyboard->typingAreaBegin();
	auto keys_end=_this->r_keyboard->typingAreaEnd();
	size_t k=0;
	size_t l=0;
	double x=0;
	while(keys!=keys_end)
		{
		size_t vertex=0;
		auto v=keys->vertexGet(vertex);
		auto x_max=v.x;
		auto color=keys->colorGet();
		auto color_border=keys->colorBorderGet();
		GdkRGBA color_gtk{color.red,color.green,color.blue,color.alpha};
		if(v.x==-1.0f)
			{goto next;} //Is there a better way...

		gdk_cairo_set_source_rgba(cr, &color_gtk);
		cairo_move_to(cr,key_width*(v.x+x),key_width*(v.y+l));
		++vertex;
		while(v.x!=-1.0f) //Drawing of keys should be done in another function
			{
			v=keys->vertexGet(vertex);
			if(v.x==-1.0f)
				{break;}
			cairo_line_to(cr,(v.x+x)*key_width,(v.y+l)*key_width);

			if(v.x>x_max)
				{x_max=v.x;}
			++vertex;
			}
		cairo_close_path(cr);
		cairo_fill_preserve (cr);
		cairo_set_line_width(cr,key_width/16);
		color_gtk={color_border.red
			,color_border.green,color_border.blue,color_border.alpha};
		gdk_cairo_set_source_rgba(cr,&color_gtk);
		cairo_stroke(cr);

		if(isLight(color))
			{color_gtk={0,0,0,1};}
		else
			{color_gtk={1,1,1,1};}
		gdk_cairo_set_source_rgba(cr, &color_gtk);

		cairo_text_extents_t extents;
		cairo_text_extents(cr,keys->labelGet(),&extents);
		cairo_move_to(cr,(x+0.5*x_max)*key_width-0.5*extents.width
			,(l+0.5)*key_width+0.5*extents.height);
		cairo_show_text(cr,keys->labelGet());
		x+=x_max;

	next:
		++k;
		++keys;
		if(k%n_cols==0)
			{
			k=0;
			x=0;
			++l;
			}
		}

	return FALSE;
	}
