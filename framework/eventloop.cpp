#ifdef __WAND__
target
	[
	name[eventloop.o]
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

#include "eventloop.h"
#include <gtk/gtk.h>

class EventLoopGtk:public EventLoop
	{
	public:
		EventLoopGtk(bool background_light);
		void run();

	private:
		static bool s_init;
	};

bool EventLoopGtk::s_init=0;

EventLoopGtk::EventLoopGtk(bool background_light)
	{
	if(!s_init)
		{
		gtk_init(0,NULL);
		g_object_set(gtk_settings_get_default(),"gtk-application-prefer-dark-theme"
			,!background_light,NULL);
		s_init=1;
		}
	}

void EventLoopGtk::run()
	{
	while(windowsLeft()!=0)
		{
		gtk_main_iteration();
		}
	delete this;
	}

EventLoop* EventLoop::instanceCreate(bool background_light)
	{
	return new EventLoopGtk(background_light);
	}
