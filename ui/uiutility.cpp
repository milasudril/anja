//@	{
//@	 "targets":[{"name":"uiutility.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	,"dependencies_extra":[{"ref":"ui_utility.o","rel":"implementation"}]
//@	}

#include "uiutility.hpp"
#include <gtk/gtk.h>

using namespace Anja;

ColorRGBA Anja::ambientColor(void* widget_handle)
	{
	auto surface=cairo_image_surface_create(CAIRO_FORMAT_ARGB32,4,4);
	auto cr=cairo_create(surface);
	while(widget_handle!=NULL)
		{
		auto context=gtk_widget_get_style_context(GTK_WIDGET(widget_handle));
		gtk_render_background(context,cr,0,0,1,1);
		cairo_surface_flush(surface);
		auto content=cairo_image_surface_get_data(surface);
		if(content[3]==255)
			{
			auto ret=ColorRGBA{content[2]/255.0f,content[1]/255.0f,content[0]/255.0f,content[3]/255.0f};
			cairo_destroy(cr);
			cairo_surface_destroy(surface);
			return ret;
			}
		widget_handle=gtk_widget_get_parent(GTK_WIDGET(widget_handle));
		}
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	return ColorRGBA{1.0f,1.0f,1.0f,1.0f};
	}
