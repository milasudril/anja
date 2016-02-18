#ifdef __WAND__
target
	[
	name[boxvertical.o]
	type[object]
	platform[;Windows]
	]
#endif

#include "boxvertical.h"
#include "guihandle.h"
#include "arraydynamicshort.h"
#include "exceptionswallow.h"
#include <algorithm>

class BoxVerticalWindows:public BoxVertical
	{
	public:
		BoxVerticalWindows(GuiContainer& parent);
		~BoxVerticalWindows();

		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_box;}

		void insertModeSet(uint32_t mode)
			{m_insert_mode=mode;}

		void componentAdd(Widget& widget);
		void componentRemove(Widget& widget);

		void slaveAssign(Widget& widget)
			{r_slave=&widget;}

		void slaveRelease()
			{
			r_slave=nullptr;
			}

		void fullscreenSet(bool state)
			{
			auto window=GetAncestor(m_box,GA_ROOT);
			if(window!=NULL)
				{
				if(state)
					{
					m_style_old=GetWindowLongPtrA(window,GWL_STYLE);
					SetWindowLongPtrA(window,GWL_STYLE,0);
					SetWindowPos(window,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE);
					ShowWindow(window,SW_MAXIMIZE);
					}
				else
					{
					SetWindowLongPtrA(window,GWL_STYLE,m_style_old);
					ShowWindow(window,SW_SHOW);
					}
				}
			}

	private:
		GuiContainer& r_parent;
		GuiHandle m_box;
		Widget* r_slave;
		WNDPROC m_proc_old;
		uint32_t m_insert_mode;
		unsigned int m_style_old;

		struct Cell
			{
			bool operator==(const Widget* w) const
				{return widget==w;}

			Widget* widget;
			uint32_t insert_mode;
			};

		ArrayDynamicShort<Cell> m_widgets;
		void componentRemoveAt(const ArrayDynamicShort<Cell>::iterator& i);

		void widgetsPack(unsigned int width,unsigned int height);

		static LRESULT CALLBACK messageHandler(HWND handle,UINT message
			,WPARAM param_0,LPARAM param_1);
	};

LRESULT CALLBACK BoxVerticalWindows::messageHandler(HWND handle,UINT message
	,WPARAM param_0,LPARAM param_1)
	{
	BoxVerticalWindows* _this=
		reinterpret_cast<BoxVerticalWindows*>(GetWindowLongPtrA(handle,GWLP_USERDATA));

	switch(message)
		{
		case WM_SIZE:
			_this->widgetsPack(LOWORD(param_1),HIWORD(param_1));
			return 0;
		}

	return CallWindowProcA(_this->m_proc_old,handle,message,param_0,param_1);
	}

BoxVertical* BoxVertical::create(GuiContainer& parent)
	{return new BoxVerticalWindows(parent);}

void BoxVerticalWindows::destroy()
	{
	if(r_slave==nullptr)
		{delete this;}
	else
		{r_slave->destroy();}
	}

BoxVerticalWindows::BoxVerticalWindows(GuiContainer& parent):
	r_parent(parent),r_slave(nullptr),m_insert_mode(0)
	{
	HWND box=CreateWindowExA(0,"AnjaWindow","BoxVertical"
		,WS_CHILD|WS_VISIBLE,0,0,16,16
		,parent.handleNativeGet()
		,NULL,GetModuleHandle(NULL)
		,NULL);
	m_box=box;

	SetWindowLongPtrA(box,GWLP_USERDATA,(LONG_PTR)this);
	m_proc_old=reinterpret_cast<WNDPROC>( SetWindowLongPtrA(box,GWLP_WNDPROC,(LONG_PTR)messageHandler) );

	r_parent.componentAdd(*this);
	}

void BoxVerticalWindows::widgetsPack(unsigned int width,unsigned int height)
	{
	//	Find largest width
	auto w_min=width;
		{
		auto ptr=m_widgets.begin();
		auto ptr_end=m_widgets.end();
		while(ptr!=ptr_end)
			{
			w_min=std::max(ptr->widget->widthMinGet(),w_min);
			++ptr;
			}
		}

	//	Find total height
	auto h_min=height;
	auto height_normal=0u;
	auto N_expanded=0u;
		{
		auto sum=0u;

		auto ptr=m_widgets.begin();
		auto ptr_end=m_widgets.end();
		while(ptr!=ptr_end)
			{
			auto height_current=ptr->widget->heightForWidthGet(w_min) + 4;
			sum+=height_current;
			if( (ptr->insert_mode&INSERTMODE_EXPAND)
				|| (ptr->insert_mode&INSERTMODE_FILL) )
				{++N_expanded;}
			else
				{height_normal+=height_current;}

			++ptr;
			}
		h_min=std::max(sum,height);
		}
	if(w_min>width || h_min>height)
		{
		MoveWindow(m_box,0,0,w_min,h_min,FALSE);
		return;
		}

	double height_shared=(height-height_normal)/N_expanded;

	//	Position regular widgets
	auto height_total=0u;
		{
		auto pos_current=0u;
		auto ptr=m_widgets.begin();
		auto ptr_end=m_widgets.end();

		while(ptr!=ptr_end)
			{
			if(!(ptr->insert_mode&INSERTMODE_END))
				{
				auto height_current=0;
				if( (ptr->insert_mode&INSERTMODE_EXPAND)
					|| (ptr->insert_mode&INSERTMODE_FILL) )
					{height_current=height_shared;}
				else
					{height_current=ptr->widget->heightForWidthGet(h_min);}

				MoveWindow(ptr->widget->handleNativeGet(),0,pos_current,w_min
					,height_current,TRUE);
				pos_current+=height_current + 4;
				}
			++ptr;
			}
		}

	//	Position end widgets
		{
		auto pos_current=height;
		auto ptr=m_widgets.begin();
		auto ptr_end=m_widgets.end();

		while(ptr!=ptr_end)
			{
			if(ptr->insert_mode&INSERTMODE_END)
				{
				auto height_current=0;
				if( (ptr->insert_mode&INSERTMODE_EXPAND)
					|| (ptr->insert_mode&INSERTMODE_FILL) )
					{height_current=height_shared;}
				else
					{height_current=ptr->widget->widthForHeightGet(h_min);}
				pos_current-=(height_current + 4);
				MoveWindow(ptr->widget->handleNativeGet(),0,pos_current,w_min
					,height_current,TRUE);
				}
			++ptr;
			}
		}
	}

BoxVerticalWindows::~BoxVerticalWindows()
	{
	if(r_slave!=nullptr)
		{r_slave->destroy();}

	auto ptr=m_widgets.end();
	auto ptr_begin=m_widgets.begin();
	while(ptr!=ptr_begin)
		{
		--ptr;
		ptr->widget->destroy();
		}
	r_parent.componentRemove(*this);
	SetWindowLongPtrA(m_box,GWLP_WNDPROC,(LONG_PTR)m_proc_old);
	DestroyWindow(m_box);
	}

void BoxVerticalWindows::componentAdd(Widget& widget)
	{
	auto h=widget.handleNativeGet();
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,&widget);
	if(i!=m_widgets.end())
		{return;}
	m_widgets.append({&widget,m_insert_mode});
	}

void BoxVerticalWindows::componentRemove(Widget& widget)
	{
	auto begin=m_widgets.begin();
	auto end=m_widgets.end();
	auto i=std::find(begin,end,&widget);
	if(i!=m_widgets.end())
		{
		componentRemoveAt(i);
		}
	}

void BoxVerticalWindows::componentRemoveAt(const ArrayDynamicShort<Cell>::iterator& i)
	{
	m_widgets.erase(i);
	}
