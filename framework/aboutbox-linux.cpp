#ifdef __WAND__
target[
	name[aboutbox.o] type[object]
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

#include "aboutbox.h"
#include "widget.h"
#include "guihandle.h"
#include "arraydynamicshort.h"
#include "programinfo.h"
#include "rgbablock.h"
#include "array_simple.h"
#include "color.h"
#include <gtk/gtk.h>
#include <cstring>


class AboutBoxGtk:public AboutBox
	{
	public:
		AboutBoxGtk(const Widget&owner,const ProgramInfo& info);

		~AboutBoxGtk();


	private:
		char* m_filename_out;
		GtkWidget* m_dialog;
		GtkWidget* m_title;
		GtkWidget* m_description;
		GtkWidget* m_authors;
		GtkWidget* m_acknowledgement_title;
		GtkWidget* m_acknowledgement_content;
		GtkWidget* m_disclaimer;
		GtkWidget* m_logo;
		GtkWidget* m_compileinfo;
		const RGBABlock* r_logo;
		static gboolean logo_draw(GtkWidget *widget, cairo_t *cr,void* aboutboxgtk);

	};

struct alignas(4) Pixel
	{
	uint8_t v0;
	uint8_t v1;
	uint8_t v2;
	uint8_t v3;
	};

gboolean AboutBoxGtk::logo_draw(GtkWidget *widget, cairo_t *cr,void* aboutboxgtk)
	{
	AboutBoxGtk* _this=reinterpret_cast<AboutBoxGtk*>(aboutboxgtk);
	if(_this->r_logo!=nullptr)
		{
		auto width=gtk_widget_get_allocated_width(widget);
		auto height=gtk_widget_get_allocated_height(widget);

		auto width_in=_this->r_logo->width;

		auto ratio_in=double(width_in)/double(_this->r_logo->height);
		auto ratio_out=double(width)/double(height);

		auto height_out=size_t(ratio_out > ratio_in? height : width/ratio_in);
		auto width_out=size_t(ratio_out > ratio_in? height*ratio_in : width);

		auto factor=double(width_in)/width_out;

		ArraySimple<Pixel> pixels(width_out*height_out);
		auto ptr=pixels.begin();
		auto ptr_end=pixels.end();
		auto ptr_src=reinterpret_cast<const Pixel*>( _this->r_logo->pixelsGet() );
		unsigned int row=0;
		unsigned int col=0;
		while(ptr!=ptr_end)
			{
		//	TODO: Interpolate
			auto row_src=size_t(row*factor);
			auto col_src=size_t(col*factor);

			auto alpha=ptr_src[row_src*width_in + col_src].v3/255.0f;

		//	Cairo requires that RGB values are multiplied by alpha
			ColorRGBA v_source
				{
				 alpha*ptr_src[row_src*width_in + col_src].v0/255.0f
				,alpha*ptr_src[row_src*width_in + col_src].v1/255.0f
				,alpha*ptr_src[row_src*width_in + col_src].v2/255.0f
				,alpha
				};

			*ptr=
				{
				 uint8_t(255*v_source.blue)
				,uint8_t(255*v_source.green)
				,uint8_t(255*v_source.red)
				,uint8_t(255*v_source.alpha)
				};

			++col;
			if(col==width_out)
				{
				col=0;
				++row;
				}
			++ptr;
			}

		auto surface=cairo_image_surface_create_for_data(
			(uint8_t*)pixels.begin()
			,CAIRO_FORMAT_ARGB32
			,width_out,height_out
			,width_out*sizeof(Pixel));
		cairo_set_operator(cr,CAIRO_OPERATOR_OVER);
		cairo_set_source_surface(cr, surface, 0.5*(width-width_out), 0.0);
		cairo_paint(cr);

		cairo_surface_destroy(surface);
		}
	return FALSE;
	}

AboutBoxGtk::AboutBoxGtk(const Widget& owner,const ProgramInfo& info):
	 m_description(nullptr),m_authors(nullptr),m_acknowledgement_title(nullptr)
	,m_acknowledgement_content(nullptr),m_disclaimer(nullptr)
	,m_logo(nullptr)
	,m_compileinfo(nullptr)
	,r_logo(info.logo)
	{
	auto parent=gtk_widget_get_toplevel(owner.handleNativeGet());
	ArrayDynamicShort<char> buffer="About ";
	buffer.truncate().append(info.name);
	m_dialog=gtk_dialog_new_with_buttons(buffer.begin(),(GtkWindow*)parent
		,GTK_DIALOG_MODAL,"OK",0,NULL);

	auto box=gtk_dialog_get_content_area( (GtkDialog*)m_dialog);
	gtk_box_set_homogeneous((GtkBox*)box,FALSE);

	if(info.logo!=nullptr)
		{
		m_logo=gtk_drawing_area_new();
		g_object_ref_sink(m_logo);
		gtk_box_pack_start((GtkBox*)box,m_logo,1,1,0);
		g_signal_connect (G_OBJECT (m_logo), "draw",
			G_CALLBACK(logo_draw),this);
		gtk_widget_set_size_request(m_logo, 0, 160);
		gtk_widget_show(m_logo);
		}

	buffer=info.name;
	if(info.version!=nullptr)
		{
		buffer.truncate().append(info.version);
		}
	m_title=gtk_label_new(buffer.begin());
	g_object_ref_sink(m_title);
	gtk_box_pack_start((GtkBox*)box,m_title,0,0,0);
	gtk_widget_show(m_title);

	if(info.description!=nullptr)
		{
		m_description=gtk_label_new(info.description);
		g_object_ref_sink(m_title);
		gtk_box_pack_start((GtkBox*)box,m_description,0,0,8);
		gtk_widget_show(m_description);
		}

	if(info.authors!=nullptr)
		{
		buffer="Written by ";
		auto ptr=info.authors;
		while(*ptr!=nullptr)
			{
			buffer.truncate().append(*ptr);
			++ptr;
			if(*ptr!=nullptr)
				{buffer.truncate().append(", ");}
			}

		char year[16];
		sprintf(year," %u",info.year);
		buffer.truncate().append(year);
		m_authors=gtk_label_new(buffer.begin());
		g_object_ref_sink(m_authors);
		gtk_box_pack_start((GtkBox*)box,m_authors,0,0,8);
		gtk_widget_show(m_authors);
		}

	m_acknowledgement_title=gtk_label_new("Acknowledgement");
	g_object_ref_sink(m_acknowledgement_title);
	gtk_box_pack_start((GtkBox*)box,m_acknowledgement_title,0,0,8);
	gtk_widget_show(m_acknowledgement_title);
	buffer="The GTK+ team - for the GTK+ library";
	if(info.acknowledgement!=nullptr)
		{
		auto ptr=info.acknowledgement;
		while(*ptr!=nullptr)
			{
			buffer.truncate().append("\n");
			buffer.truncate().append(*ptr);
			++ptr;
			}
		}
	m_acknowledgement_content=gtk_label_new(buffer.begin());
	gtk_box_pack_start((GtkBox*)box,m_acknowledgement_content,0,0,0);
	gtk_widget_show(m_acknowledgement_content);

	if(info.disclaimer!=nullptr)
		{
		m_disclaimer=gtk_label_new(info.disclaimer);
		g_object_ref_sink(m_disclaimer);
		gtk_box_pack_start((GtkBox*)box,m_disclaimer,0,0,8);
		gtk_widget_show(m_disclaimer);
		}

	if(info.compileinfo!=nullptr)
		{
		buffer="This Anja was compiled ";
		auto ptr=info.compileinfo;
		while(*ptr!=nullptr)
			{
			buffer.truncate().append(*ptr);
			++ptr;
			}
		m_compileinfo=gtk_label_new(buffer.begin());
		g_object_ref_sink(m_compileinfo);
		gtk_box_pack_start((GtkBox*)box,m_compileinfo,0,0,8);
		gtk_widget_show(m_compileinfo);
		}

	gtk_dialog_run(GTK_DIALOG(m_dialog));
	}

AboutBoxGtk::~AboutBoxGtk()
	{
	if(m_logo!=nullptr)
		{gtk_widget_destroy(m_logo);}
	if(m_compileinfo!=nullptr)
		{gtk_widget_destroy(m_compileinfo);}
	if(m_disclaimer!=nullptr)
		{gtk_widget_destroy(m_disclaimer);}
	gtk_widget_destroy(m_acknowledgement_content);
	gtk_widget_destroy(m_acknowledgement_title);
	if(m_authors!=nullptr)
		{gtk_widget_destroy(m_authors);}
	if(m_description!=nullptr)
		{gtk_widget_destroy(m_description);}
	gtk_widget_destroy(m_title);
	gtk_widget_destroy(m_dialog);
	}

AboutBox* AboutBox::instanceCreate(const Widget& owner,const ProgramInfo& info)
	{return new AboutBoxGtk(owner,info);}

void AboutBox::destroy(AboutBox* aboutbox)
	{
	delete dynamic_cast<AboutBoxGtk*>(aboutbox);
	}
