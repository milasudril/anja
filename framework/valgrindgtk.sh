#!/bin/bash

valgrind --tool=memcheck --leak-check=full \
	--suppressions=/usr/local/share/gtkvalgrind/base.supp \
	--suppressions=/usr/local/share/gtkvalgrind/gail.supp \
	--suppressions=/usr/local/share/gtkvalgrind/gdk.supp \
	--suppressions=/usr/local/share/gtkvalgrind/gio.supp \
	--suppressions=/usr/local/share/gtkvalgrind/glib.supp \
	--suppressions=/usr/local/share/gtkvalgrind/gtk3.supp \
	--suppressions=/usr/local/share/gtkvalgrind/gtksourceview.supp \
	--suppressions=/usr/local/share/gtkvalgrind/gtk.supp \
	--suppressions=/usr/local/share/gtkvalgrind/pango.supp \
	"$1"
