#ifdef __WAND__
target
	[
	name[label.o]
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

#include "label.h"
#include "guicontainer.h"
#include "guihandle.h"

class LabelGtk:public Label
	{
	public:
		LabelGtk(GuiContainer& parent,const char* title);
		~LabelGtk();
		void destroy();
		const GuiHandle& handleNativeGet() const
			{return m_label;}

	private:
		GuiContainer& r_parent;
		GuiHandle m_label;
	};

Label* Label::create(GuiContainer& parent,const char* title)
	{return new LabelGtk(parent,title);}

void LabelGtk::destroy()
	{delete this;}

LabelGtk::LabelGtk(GuiContainer& parent,const char* title):r_parent(parent)
	{
	GtkWidget* widget=gtk_label_new(title);
	m_label=widget;
	g_object_ref_sink(widget);
	parent.componentAdd(*this);
	}

LabelGtk::~LabelGtk()
	{
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_label);
	}
