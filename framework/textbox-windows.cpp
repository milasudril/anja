#ifdef __WAND__
target
	[
	name[textbox.o]
	type[object]
	platform[;Windows]
	]
#endif

#include "textbox.h"
#include "guicontainer.h"
#include "guihandle.h"
#include "exceptionswallow.h"

#include "array_simple.h"

class TextboxWindows:public Textbox
	{
	public:
		TextboxWindows(GuiContainer& parent,EventHandler& handler
			,unsigned int id);
		~TextboxWindows();
		void destroy();

		const GuiHandle& handleNativeGet() const
			{return m_textbox;}

		const char* textGet() const
			{
			m_text=GetWindowTextLengthA(m_textbox)+1;
			GetWindowTextA(m_textbox,m_text.begin(),m_text.length());
			return m_text.begin();
			}

		void textSet(const char* text)
			{SetWindowTextA(m_textbox,text);}

		void widthMinSet(unsigned int nChars)
			{
		//	TODO
			}

		unsigned int idGet() const
			{return GetWindowLongPtrA(m_textbox,GWLP_ID);}

	private:
		GuiContainer& r_parent;
		EventHandler* r_handler;
		GuiHandle m_textbox;
		mutable ArraySimple<char> m_text;
	};

Textbox::EventHandler Textbox::s_default_handler;

Textbox* Textbox::create(GuiContainer& parent,EventHandler& handler
	,unsigned int id)
	{return new TextboxWindows(parent,handler,id);}

void TextboxWindows::destroy()
	{delete this;}

TextboxWindows::TextboxWindows(GuiContainer& parent,EventHandler& handler
	,unsigned int id):
	r_parent(parent),r_handler(&handler),m_text(256)
	{
	m_textbox=CreateWindowExA(WS_EX_CLIENTEDGE
		,"EDIT",""
		,WS_CHILD|WS_VISIBLE,0,0,96,24
		,parent.handleNativeGet()
		,(HMENU)size_t(id),GetModuleHandle(NULL)
		,NULL);

//	TODO: Overload winproc
	parent.componentAdd(*this);
	}

TextboxWindows::~TextboxWindows()
	{
	r_parent.componentRemove(*this);
	DestroyWindow(m_textbox);
	}
