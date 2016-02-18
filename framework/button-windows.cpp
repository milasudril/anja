#ifdef __WAND__
target
	[
	name[button.o]
	type[object]
	platform[;Windows]
	]
#endif

#include "button.h"
#include "guicontainer.h"
#include "guihandle.h"
#include "exceptionswallow.h"

#include "array_simple.h"

class ButtonWindows:public Button
	{
	public:
		ButtonWindows(GuiContainer& parent,EventHandler& handler
			,unsigned int id,const char* title);
		~ButtonWindows();
		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_button;}

		void titleSet(const char* text)
			{SetWindowTextA(m_button,text);}

		unsigned int idGet() const
			{return GetWindowLongPtrA(m_button,GWLP_ID);}

	private:
		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_button;
	};

Button::EventHandler Button::s_default_handler;

Button* Button::create(GuiContainer& parent,EventHandler& handler
	,unsigned int id,const char* title)
	{return new ButtonWindows(parent,handler,id,title);}

void ButtonWindows::destroy()
	{delete this;}

ButtonWindows::ButtonWindows(GuiContainer& parent,EventHandler& handler
	,unsigned int id,const char* title):
	r_parent(parent),r_handler(&handler)
	{
	m_button=CreateWindowExA(WS_EX_CLIENTEDGE
		,"BUTTON",title
		,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,0,0,96,24
		,parent.handleNativeGet()
		,(HMENU)size_t(id),GetModuleHandle(NULL)
		,NULL);

//	TODO: Overload winproc
	parent.componentAdd(*this);
	}

ButtonWindows::~ButtonWindows()
	{
	r_parent.componentRemove(*this);
	DestroyWindow(m_button);
	}
