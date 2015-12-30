#ifdef __WAND__
target
	[
	name[colorpicker.o]
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

#include "colorpicker.h"
#include "color.h"
#include "colorsystem.h"
#include "guihandle.h"
#include "array_simple.h"
#include "guicontainer.h"

#include <gtk/gtk.h>

#include <cstring>
#include <algorithm>

class ColorPickerGtk:public ColorPicker
	{
	public:
		ColorPickerGtk(GuiContainer& parent,ColorRGBA& color
			,ColorRGBA* presets,size_t N_presets);

		~ColorPickerGtk();

		void destroy()
			{delete this;}

		const GuiHandle& handleNativeGet() const
			{return m_handle;}

		void update()
			{
			colorSamlpesUpdate();
			slidersUpdate();
			}

	private:
		GuiHandle m_handle;

		GtkWidget* m_aspect_box;
			GtkWidget* m_presets_tab;

		GtkWidget* m_hsltab;
			GtkWidget* m_h_slider;

			GtkWidget* m_s_min;
			GtkWidget* m_s_slider;
			GtkWidget* m_s_max;

			GtkWidget* m_l_min;
			GtkWidget* m_l_slider;
			GtkWidget* m_l_max;

		GtkWidget* m_result_box;
			GtkWidget* m_result_label;
			GtkWidget* m_result_sample;
			GtkWidget* m_result_rgb;

		GtkWidget* m_exit_box;
			GtkWidget* m_ok;
			GtkWidget* m_cancel;

		ArraySimple<ColorRGBA> m_presets;
		ColorRGBA* r_presets;
		ColorRGBA* r_color;
		ColorRGBA* r_color_active;
		GuiContainer& r_parent;
		double m_size_colorbox;

		static gboolean tabOnPaint(GtkWidget* object,cairo_t* cr
			,void* colorpickergtk);
		static gboolean sMinOnPaint(GtkWidget* object,cairo_t* cr
			,void* colorpickergtk);
		static gboolean sMaxOnPaint(GtkWidget* object,cairo_t* cr
			,void* colorpickergtk);
			static gboolean lMinOnPaint(GtkWidget* object,cairo_t* cr
			,void* colorpickergtk);
		static gboolean lMaxOnPaint(GtkWidget* object,cairo_t* cr
			,void* colorpickergtk);
		static gboolean resultOnPaint(GtkWidget* object,cairo_t* cr
			,void* colorpickergtk);
		static gboolean onPresetActivate(GtkWidget* object,GdkEventMotion* event
			,void* colorpickergtk);

		static void sliderChange(GtkRange* range,void* colorpickergtk);
		static void saveAndExit(GtkButton* button,void* colorpickergtk);
		static void exit(GtkButton* button,void* colorpickergtk);

		void slidersUpdate();
		void colorSamlpesUpdate();

	};

ColorPicker* ColorPicker::create(GuiContainer& parent,ColorRGBA& color
	,ColorRGBA* presets,size_t N_presets)
	{
	return new ColorPickerGtk(parent,color,presets,N_presets);
	}


ColorPickerGtk::ColorPickerGtk(GuiContainer& parent,ColorRGBA& color
	,ColorRGBA* presets,size_t N_presets):
	m_presets(std::max(N_presets,size_t(64))),r_presets(presets)
 	,r_color(&color),r_color_active(&color),r_parent(parent)
	{
	memcpy(m_presets.begin(),presets,N_presets*sizeof(ColorRGBA));
	GtkWidget* box=gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
	g_object_ref_sink(box);
	m_handle=box;
	parent.componentAdd(*this);

	m_aspect_box=gtk_aspect_frame_new("Presets:",0.5f,0.5f,16.0f/4.0f,TRUE);
	g_object_ref_sink(m_aspect_box);
	gtk_box_pack_start((GtkBox*)box,m_aspect_box,TRUE,TRUE,0);
	gtk_frame_set_shadow_type ((GtkFrame*)m_aspect_box,GTK_SHADOW_NONE);
	gtk_frame_set_label_align ((GtkFrame*)m_aspect_box,0.0f,1.0f);
	gtk_widget_show(m_aspect_box);

	m_presets_tab=gtk_drawing_area_new();
	g_object_ref_sink(m_presets_tab);
	gtk_widget_add_events(m_presets_tab,GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK);
	g_signal_connect(m_presets_tab,"draw",G_CALLBACK(tabOnPaint),this);
	g_signal_connect(m_presets_tab,"button-release-event"
		,G_CALLBACK(onPresetActivate),this);
	gtk_container_add ((GtkContainer*)m_aspect_box,m_presets_tab);
	gtk_widget_set_size_request(m_presets_tab,24*16,24*m_presets.length()/16);
	gtk_widget_show(m_presets_tab);

	m_hsltab=gtk_grid_new();
	g_object_ref_sink(m_hsltab);
	gtk_box_pack_start((GtkBox*)box,m_hsltab,FALSE,FALSE,0);
	gtk_grid_set_row_spacing ((GtkGrid*)m_hsltab,4);
	gtk_widget_show(m_hsltab);
		m_h_slider=gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,1,1e-3);
		g_object_ref_sink(m_h_slider);
		gtk_scale_set_has_origin((GtkScale*)m_h_slider,FALSE);
		gtk_scale_set_draw_value((GtkScale*)m_h_slider,FALSE);
		gtk_widget_set_hexpand(m_h_slider,TRUE);
		g_signal_connect(m_h_slider,"value-changed",G_CALLBACK(sliderChange),this);
		gtk_grid_attach((GtkGrid*)m_hsltab,m_h_slider,0,0,3,1);
		gtk_widget_show(m_h_slider);

		m_s_min=gtk_drawing_area_new();
		g_object_ref_sink(m_s_min);
		gtk_widget_set_size_request(m_s_min,32,32);
		gtk_grid_attach((GtkGrid*)m_hsltab,m_s_min,0,1,1,1);
		g_signal_connect(m_s_min,"draw",G_CALLBACK(sMinOnPaint),this);
		gtk_widget_show(m_s_min);
		m_s_slider=gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,1,1e-3);
		g_object_ref_sink(m_s_slider);
		gtk_scale_set_has_origin((GtkScale*)m_s_slider,FALSE);
		gtk_scale_set_draw_value((GtkScale*)m_s_slider,FALSE);
		gtk_widget_set_hexpand(m_s_slider,TRUE);
		gtk_grid_attach((GtkGrid*)m_hsltab,m_s_slider,1,1,1,1);
		g_signal_connect(m_s_slider,"value-changed",G_CALLBACK(sliderChange),this);
		gtk_widget_show(m_s_slider);
		m_s_max=gtk_drawing_area_new();
		g_object_ref_sink(m_s_max);
		gtk_widget_set_size_request(m_s_max,32,32);
		gtk_grid_attach((GtkGrid*)m_hsltab,m_s_max,2,1,1,1);
		g_signal_connect(m_s_max,"draw",G_CALLBACK(sMaxOnPaint),this);
		gtk_widget_show(m_s_max);

		m_l_min=gtk_drawing_area_new();
		g_object_ref_sink(m_l_min);
		gtk_widget_set_size_request(m_l_min,32,32);
		gtk_grid_attach((GtkGrid*)m_hsltab,m_l_min,0,2,1,1);
		g_signal_connect(m_l_min,"draw",G_CALLBACK(lMinOnPaint),this);
		gtk_widget_show(m_l_min);
		m_l_slider=gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,1,1e-3);
		g_object_ref_sink(m_l_slider);
		gtk_scale_set_has_origin((GtkScale*)m_l_slider,FALSE);
		gtk_scale_set_draw_value((GtkScale*)m_l_slider,FALSE);
		gtk_widget_set_hexpand(m_l_slider,TRUE);
		gtk_grid_attach((GtkGrid*)m_hsltab,m_l_slider,1,2,1,1);
		g_signal_connect(m_l_slider,"value-changed",G_CALLBACK(sliderChange),this);
		gtk_widget_show(m_l_slider);
		m_l_max=gtk_drawing_area_new();
		g_object_ref_sink(m_l_max);
		gtk_widget_set_size_request(m_l_max,32,32);
		gtk_grid_attach((GtkGrid*)m_hsltab,m_l_max,2,2,1,1);
		g_signal_connect(m_l_max,"draw",G_CALLBACK(lMaxOnPaint),this);
		gtk_widget_show(m_l_max);

	m_result_box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
	g_object_ref_sink(m_result_box);
	gtk_box_pack_start((GtkBox*)box,m_result_box,FALSE,FALSE,0);
	gtk_widget_show(m_result_box);
		m_result_label=gtk_label_new("Result:");
		g_object_ref_sink(m_result_label);
		gtk_box_pack_start((GtkBox*)m_result_box,m_result_label,FALSE,FALSE,0);
		gtk_widget_show(m_result_label);

		m_result_sample=gtk_drawing_area_new();
		g_object_ref_sink(m_result_sample);
		gtk_box_pack_start((GtkBox*)m_result_box,m_result_sample,TRUE,TRUE,0);
		g_signal_connect(m_result_sample,"draw",G_CALLBACK(resultOnPaint),this);
		gtk_widget_show(m_result_sample);

		m_result_rgb=gtk_label_new("");
		g_object_ref_sink(m_result_rgb);
		gtk_box_pack_end((GtkBox*)m_result_box,m_result_rgb,FALSE,FALSE,0);
		gtk_widget_show(m_result_rgb);

	m_exit_box=gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	g_object_ref_sink(m_exit_box);
	gtk_box_pack_end((GtkBox*)box,m_exit_box,FALSE,FALSE,0);
	gtk_widget_show(m_exit_box);
	gtk_box_set_homogeneous((GtkBox*)m_exit_box,TRUE);
	gtk_box_set_spacing ((GtkBox*)m_exit_box,4);
	gtk_button_box_set_layout((GtkButtonBox*)m_exit_box,GTK_BUTTONBOX_END);
		m_ok=gtk_button_new_with_label("OK");
		g_object_ref_sink(m_ok);
		gtk_box_pack_start((GtkBox*)m_exit_box,m_ok,FALSE,FALSE,0);
		gtk_widget_show(m_ok);
		g_signal_connect(m_ok,"clicked",G_CALLBACK(saveAndExit),this);
			{
			GtkWidget* toplevel=gtk_widget_get_toplevel(m_ok);
			if (gtk_widget_is_toplevel (toplevel))
				{
				gtk_widget_set_can_default(m_ok,TRUE);
				gtk_widget_grab_default(m_ok);
				}
			}
		m_cancel=gtk_button_new_with_label("Cancel");
		g_object_ref_sink(m_cancel);
		gtk_box_pack_start((GtkBox*)m_exit_box,m_cancel,FALSE,FALSE,0);
		gtk_widget_show(m_cancel);
		g_signal_connect(m_cancel,"clicked",G_CALLBACK(exit),this);

	slidersUpdate();
	colorSamlpesUpdate();
	}

ColorPickerGtk::~ColorPickerGtk()
	{
	gtk_widget_destroy(m_cancel);
	gtk_widget_destroy(m_ok);
	gtk_widget_destroy(m_exit_box);

	gtk_widget_destroy(m_result_rgb);
	gtk_widget_destroy(m_result_sample);
	gtk_widget_destroy(m_result_label);
	gtk_widget_destroy(m_result_box);

	gtk_widget_destroy(m_l_max);
	gtk_widget_destroy(m_l_slider);
	gtk_widget_destroy(m_l_min);

	gtk_widget_destroy(m_s_max);
	gtk_widget_destroy(m_s_slider);
	gtk_widget_destroy(m_s_min);

	gtk_widget_destroy(m_h_slider);
	gtk_widget_destroy(m_hsltab);
	gtk_widget_destroy(m_presets_tab);
	gtk_widget_destroy(m_aspect_box);
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_handle);
	}

void ColorPickerGtk::slidersUpdate()
	{
	ColorHSLA color=*r_color_active;
	gtk_range_set_value((GtkRange*)m_h_slider,color.hue);
	gtk_range_set_value((GtkRange*)m_s_slider,color.saturation);
	gtk_range_set_value((GtkRange*)m_l_slider,color.lightness);
	}

void ColorPickerGtk::colorSamlpesUpdate()
	{
	gtk_widget_queue_draw(m_presets_tab);
	gtk_widget_queue_draw(m_s_min);
	gtk_widget_queue_draw(m_s_max);
	gtk_widget_queue_draw(m_result_sample);

	char buffer[64];
	sprintf(buffer,"%.7f;\n%.7f;\n%.7f",r_color_active->red
		,r_color_active->green
		,r_color_active->blue);
	gtk_label_set_text((GtkLabel*)m_result_rgb,buffer);
	}

void ColorPickerGtk::sliderChange(GtkRange* range, void* colorpickergtk)
	{
	ColorPickerGtk* _this=(ColorPickerGtk*)colorpickergtk;
	ColorHSLA color=*(_this->r_color_active);
	color.hue=gtk_range_get_value((GtkRange*)_this->m_h_slider);
	color.saturation=gtk_range_get_value((GtkRange*)_this->m_s_slider);
	color.lightness=gtk_range_get_value((GtkRange*)_this->m_l_slider);
	*(_this->r_color_active)=color;
	_this->colorSamlpesUpdate();
	}

gboolean ColorPickerGtk::tabOnPaint(GtkWidget* object,cairo_t* cr,void* colorpickergtk)
	{
	ColorPickerGtk* _this=(ColorPickerGtk*)colorpickergtk;

	auto width = gtk_widget_get_allocated_width (object);
	const uint32_t N=16;
	auto size=width/N;
	_this->m_size_colorbox=size;

	auto ptr=_this->m_presets.begin();
	auto ptr_end=_this->m_presets.end();
	cairo_set_line_width(cr,2);
	uint32_t k=0;
	uint32_t l=0;
	while(ptr!=ptr_end)
		{
		cairo_rectangle(cr,k*size,l*size,size,size);
		gdk_cairo_set_source_rgba(cr,ColorSystem(*ptr));
		cairo_fill_preserve(cr);
		if(ptr==_this->r_color_active)
			{cairo_set_source_rgb(cr,1,1,1);}
		else
			{cairo_set_source_rgb(cr,0,0,0);}
		cairo_stroke(cr);
		++k;
		if(k%N==0)
			{k=0;++l;}
		++ptr;
		}
	return FALSE;
	}

gboolean ColorPickerGtk::sMinOnPaint(GtkWidget* object,cairo_t* cr,void* colorpickergtk)
	{
	ColorPickerGtk* _this=(ColorPickerGtk*)colorpickergtk;
	auto size = gtk_widget_get_allocated_width (object);
	cairo_rectangle(cr,0,0,size,size);
	ColorHSLA color_tmp=*(_this->r_color_active);
	color_tmp.saturation=0.0f;
	gdk_cairo_set_source_rgba(cr,ColorSystem{color_tmp});
	cairo_fill_preserve(cr);
	cairo_set_source_rgb(cr,0,0,0);
	cairo_stroke(cr);
	return FALSE;
	}

gboolean ColorPickerGtk::sMaxOnPaint(GtkWidget* object,cairo_t* cr,void* colorpickergtk)
	{
	ColorPickerGtk* _this=(ColorPickerGtk*)colorpickergtk;
	auto size = gtk_widget_get_allocated_width (object);
	cairo_rectangle(cr,0,0,size,size);
	ColorHSLA color_tmp=*(_this->r_color_active);
	color_tmp.saturation=1.0f;
	color_tmp.hue=gtk_range_get_value((GtkRange*)_this->m_h_slider);
	gdk_cairo_set_source_rgba(cr,ColorSystem{color_tmp});
	cairo_fill_preserve(cr);
	cairo_set_source_rgb(cr,0,0,0);
	cairo_stroke(cr);
	return FALSE;
	}

gboolean ColorPickerGtk::lMinOnPaint(GtkWidget* object,cairo_t* cr,void* colorpickergtk)
	{
	ColorPickerGtk* _this=(ColorPickerGtk*)colorpickergtk;
	auto size = gtk_widget_get_allocated_width (object);
	cairo_rectangle(cr,0,0,size,size);
	ColorHSLA color_tmp=*(_this->r_color_active);
	color_tmp.lightness=0.0f;
	gdk_cairo_set_source_rgba(cr,ColorSystem{color_tmp});
	cairo_fill_preserve(cr);
	cairo_set_source_rgb(cr,0,0,0);
	cairo_stroke(cr);
	return FALSE;
	}

gboolean ColorPickerGtk::lMaxOnPaint(GtkWidget* object,cairo_t* cr,void* colorpickergtk)
	{
	ColorPickerGtk* _this=(ColorPickerGtk*)colorpickergtk;
	auto size = gtk_widget_get_allocated_width (object);
	cairo_rectangle(cr,0,0,size,size);
	ColorHSLA color_tmp=*(_this->r_color_active);
	color_tmp.lightness=1.0f;
	gdk_cairo_set_source_rgba(cr,ColorSystem{color_tmp});
	cairo_fill_preserve(cr);
	cairo_set_source_rgb(cr,0,0,0);
	cairo_stroke(cr);
	return FALSE;
	}

gboolean ColorPickerGtk::resultOnPaint(GtkWidget* object,cairo_t* cr,void* colorpickergtk)
	{
	ColorPickerGtk* _this=(ColorPickerGtk*)colorpickergtk;
	auto width = gtk_widget_get_allocated_width (object);
	auto height= gtk_widget_get_allocated_height (object);
	cairo_rectangle(cr,0,0,width,height);
	gdk_cairo_set_source_rgba(cr,ColorSystem{*_this->r_color_active});
	cairo_fill_preserve(cr);
	cairo_set_source_rgb(cr,0,0,0);
	cairo_stroke(cr);
	return FALSE;
	}

gboolean ColorPickerGtk::onPresetActivate(GtkWidget* object
	,GdkEventMotion* event,void* colorpickergtk)
	{
	ColorPickerGtk* _this=(ColorPickerGtk*)colorpickergtk;
	auto x=event->x;
	auto y=event->y;
	auto size=_this->m_size_colorbox;
	if(x>=0 && x<=size*16 && y>=0 && y<=size*_this->m_presets.length()/16)
		{
		auto index=size_t(x/size) + 16*size_t(y/size);
		_this->r_color_active=&_this->m_presets[index];
		_this->slidersUpdate();
		_this->update();
		}
	return TRUE;
	}

void ColorPickerGtk::saveAndExit(GtkButton* button,void* colorpickergtk)
	{
	ColorPickerGtk* _this=(ColorPickerGtk*)colorpickergtk;
	*(_this->r_color)=*(_this->r_color_active);
	memcpy(_this->r_presets,_this->m_presets.begin()
		,_this->m_presets.length()*sizeof(ColorRGBA));
	_this->r_parent.destroy();
	}

void ColorPickerGtk::exit(GtkButton* button,void* colorpickergtk)
	{
	ColorPickerGtk* _this=(ColorPickerGtk*)colorpickergtk;
	_this->r_parent.destroy();
	}
