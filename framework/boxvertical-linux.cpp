#ifdef __WAND__
target
	[
	name[boxvertical.o]
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

#include "boxvertical.h"
#include "guihandle.h"
#include <vector>
#include <algorithm>

class BoxVerticalGtk:public BoxVertical
	{
	public:
		BoxVerticalGtk(GuiContainer& parent,EventHandler* event_handler);

		void destroy()
			{r_parent.componentRemove(*this);}

		const GuiHandle& handleNativeGet() const
			{return m_box;}

		void insertModeSet(uint32_t mode)
			{m_insert_mode=mode;}

		void componentAdd(const Widget& widget);
		void componentRemove(const Widget& widget);

		void commandNotify(unsigned int command_id)
			{
			if(r_handler==nullptr)
				{r_parent.commandNotify(command_id);}
			else
				{r_handler->onCommand(command_id);}
			}

	private:
		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_box;
		uint32_t m_insert_mode;

		std::vector<GuiHandle> m_widgets;
		void componentRemoveAt(const std::vector<GuiHandle>::iterator& i)
			{
			GtkWidget* widget=m_box;
			gtk_container_remove(GTK_CONTAINER(widget),*i);
			m_widgets.erase(i);
			}

		static void onDestroy(GtkWidget* widget,void* boxverticalgtk)
			{
			BoxVerticalGtk* _this=(BoxVerticalGtk*)boxverticalgtk;
			if(_this->r_handler!=nullptr)
				{_this->r_handler->onDestroy();}
			delete _this;
			}
	};

BoxVertical* BoxVertical::create(GuiContainer& parent,EventHandler* handler)
	{return new BoxVerticalGtk(parent,handler);}

BoxVerticalGtk::BoxVerticalGtk(GuiContainer& parent,EventHandler* handler):
	r_parent(parent),r_handler(handler),m_insert_mode(0)
	{
	GtkWidget* box=gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
	g_signal_connect(box,"destroy",G_CALLBACK(onDestroy),this);
	m_box=box;
	r_parent.componentAdd(*this);
	gtk_widget_show(box);
	}

void BoxVerticalGtk::componentAdd(const Widget& widget)
	{
	auto h=widget.handleNativeGet();
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,h);
	if(i!=m_widgets.end())
		{return;}

	m_widgets.push_back(h);
	auto insert_mode=m_insert_mode;
	GtkWidget* box=m_box;
	if(insert_mode&INSERTMODE_END)
		{
		gtk_box_pack_end((GtkBox*)box,h,insert_mode&INSERTMODE_EXPAND
			,insert_mode&INSERTMODE_FILL,0);
		}
	else
		{
		gtk_box_pack_start((GtkBox*)box,h,insert_mode&INSERTMODE_EXPAND
			,insert_mode&INSERTMODE_FILL,0);
		}
	gtk_widget_show(h);
	}

void BoxVerticalGtk::componentRemove(const Widget& widget)
	{
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,widget.handleNativeGet());
	if(i!=m_widgets.end())
		{
		componentRemoveAt(i);
		}
	}
