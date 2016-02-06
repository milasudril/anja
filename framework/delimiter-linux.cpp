#ifdef __WAND__
target
	[
	name[delimiter.o]
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

#include "delimiter.h"
#include "guihandle.h"
#include "boxvertical.h"
#include "boxhorizontal.h"

class DelimiterGtk:public Delimiter
	{
	public:
		DelimiterGtk(GuiContainer& parent,bool horizontal);
		~DelimiterGtk();
		void destroy();
		const GuiHandle& handleNativeGet() const
			{return m_delimiter;}

	private:
		GuiContainer& r_parent;
		GuiHandle m_delimiter;
	};

Delimiter* Delimiter::create(BoxVertical& parent)
	{return new DelimiterGtk(parent,1);}

Delimiter* Delimiter::create(BoxHorizontal& parent)
	{return new DelimiterGtk(parent,0);}

void DelimiterGtk::destroy()
	{delete this;}

DelimiterGtk::DelimiterGtk(GuiContainer& parent,bool horizontal):r_parent(parent)
	{
	auto orientation=horizontal?GTK_ORIENTATION_HORIZONTAL:GTK_ORIENTATION_VERTICAL;
	GtkWidget* widget=gtk_separator_new(orientation);
	m_delimiter=widget;
	g_object_ref_sink(widget);
	parent.componentAdd(*this);
	}

DelimiterGtk::~DelimiterGtk()
	{
	r_parent.componentRemove(*this);
	gtk_widget_destroy(m_delimiter);
	}
