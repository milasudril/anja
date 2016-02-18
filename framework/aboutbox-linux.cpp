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
#include <gtk/gtk.h>


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
		GtkWidget* m_build_date;
	};

AboutBoxGtk::AboutBoxGtk(const Widget& owner,const ProgramInfo& info):
	 m_description(nullptr),m_authors(nullptr),m_acknowledgement_title(nullptr)
	,m_acknowledgement_content(nullptr),m_disclaimer(nullptr),m_build_date(nullptr)
	{
	auto parent=gtk_widget_get_toplevel(owner.handleNativeGet());
	ArrayDynamicShort<char> buffer="About ";
	buffer.truncate().append(info.name);
	m_dialog=gtk_dialog_new_with_buttons(buffer.begin(),(GtkWindow*)parent
		,GTK_DIALOG_MODAL,"OK",NULL);

	auto box=gtk_dialog_get_content_area( (GtkDialog*)m_dialog);
	gtk_box_set_homogeneous((GtkBox*)box,FALSE);


	m_title=gtk_label_new(info.name);
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
		gtk_box_pack_start((GtkBox*)box,m_authors,0,0,16);
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
		gtk_box_pack_start((GtkBox*)box,m_disclaimer,1,1,8);
		gtk_widget_show(m_disclaimer);
		}

	if(info.build_date!=nullptr)
		{
		buffer="This Anja was compiled ";
		buffer.truncate().append(info.build_date);
		m_build_date=gtk_label_new(buffer.begin());
		g_object_ref_sink(m_build_date);
		gtk_box_pack_start((GtkBox*)box,m_build_date,1,1,8);
		gtk_widget_show(m_build_date);
		}


	gtk_dialog_run(GTK_DIALOG(m_dialog));
	}

AboutBoxGtk::~AboutBoxGtk()
	{
	if(m_build_date!=nullptr)
		{gtk_widget_destroy(m_build_date);}
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
