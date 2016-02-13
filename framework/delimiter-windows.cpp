#ifdef __WAND__
target
	[
	name[delimiter.o]
	type[object]
	platform[;Windows]
	]
#endif

#include "delimiter.h"
#include "guihandle.h"
#include "boxvertical.h"
#include "boxhorizontal.h"

class DelimiterWindows:public Delimiter
	{
	public:
		DelimiterWindows(GuiContainer& parent,bool horizontal);
		~DelimiterWindows();
		void destroy();
		const GuiHandle& handleNativeGet() const
			{return m_delimiter;}

	private:
		GuiContainer& r_parent;
		GuiHandle m_delimiter;
		WNDPROC m_proc_old;
		bool m_horz;

		void draw(HDC dc);

		static LRESULT CALLBACK messageHandler(HWND handle,UINT message
			,WPARAM param_0,LPARAM param_1);
	};

Delimiter* Delimiter::create(BoxVertical& parent)
	{return new DelimiterWindows(parent,1);}

Delimiter* Delimiter::create(BoxHorizontal& parent)
	{return new DelimiterWindows(parent,0);}

void DelimiterWindows::destroy()
	{delete this;}

void DelimiterWindows::draw(HDC dc)
	{
	RECT rect_client;
	GetClientRect(m_delimiter,&rect_client);
	auto pen=CreatePen(PS_SOLID,1,RGB(0,0,0));
	auto pen_old=SelectObject(dc,pen);
	if(m_horz)
		{
		auto x_0=0;
		auto x_1=rect_client.right;
		auto y=rect_client.bottom/2;
		MoveToEx(dc,x_0,y,NULL);
		LineTo(dc,x_1,y);
		}
	else
		{
		auto y_0=0;
		auto y_1=rect_client.bottom;
		auto x=rect_client.right/2;
		MoveToEx(dc,x,y_0,NULL);
		LineTo(dc,x,y_1);
		}
	SelectObject(dc,pen_old);
	DeleteObject(pen);
	}


LRESULT CALLBACK DelimiterWindows::messageHandler(HWND handle,UINT message
	,WPARAM param_0,LPARAM param_1)
	{
	DelimiterWindows* _this=
		reinterpret_cast<DelimiterWindows*>(GetWindowLongPtrA(handle,GWLP_USERDATA));

	switch(message)
		{
		case WM_PAINT:
			{
			PAINTSTRUCT ps;
			auto dc=BeginPaint(handle,&ps);
			_this->draw(dc);
			EndPaint(handle,&ps);
			}
			return 0;
		}

	return CallWindowProcA(_this->m_proc_old,handle,message,param_0,param_1);
	}

DelimiterWindows::DelimiterWindows(GuiContainer& parent,bool horizontal):r_parent(parent)
	,m_horz(horizontal)
	{
	HWND widget=CreateWindowExA(0,"AnjaWindow","Delimiter"
		,WS_CHILD|WS_VISIBLE,0,0,16,16
		,parent.handleNativeGet()
		,NULL,GetModuleHandle(NULL)
		,NULL);
	m_delimiter=widget;
	SetWindowLongPtrA(widget,GWLP_USERDATA,(LONG_PTR)this);
	m_proc_old=reinterpret_cast<WNDPROC>( SetWindowLongPtrA(widget,GWLP_WNDPROC,(LONG_PTR)messageHandler) );
	parent.componentAdd(*this);
	}

DelimiterWindows::~DelimiterWindows()
	{
	r_parent.componentRemove(*this);
	SetWindowLongPtrA(m_delimiter,GWLP_WNDPROC,(LONG_PTR)m_proc_old);
	DestroyWindow(m_delimiter);
	}
