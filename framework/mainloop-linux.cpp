#ifdef __WAND__
target
	[
	name[mainloop.o]
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

#include "mainloop.h"
#include <gtk/gtk.h>

void init(int& argc,char**& argv)
	{gtk_init(&argc,&argv);}

void run()
	{gtk_main();}
