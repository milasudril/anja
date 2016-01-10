#ifdef __WAND__
target[name[inputentry.o] type[object]]
#endif

#include "inputentry.h"
#include "button.h"
#include "textbox.h"
#include "label.h"
#include "boxhorizontal.h"

#include <cstdio>

InputEntry::EventHandlerInternal::EventHandlerInternal(InputEntry& object
	,InputEntry::EventHandler& handler):
	r_handler(handler),r_object(object)
	{}

void InputEntry::EventHandlerInternal::onCommand(BoxHorizontal& source,unsigned int command_id)
	{
	switch(command_id)
		{
		case COMMAND_ACTION:
			r_handler.onButtonClick(r_object);
			break;
		case COMMAND_TEXTCHANGED:
			r_handler.onTextChanged(r_object);
			break;
		}
	}

InputEntry::EventHandler InputEntry::s_default_handler;

InputEntry::InputEntry(GuiContainer& parent,const char* label
	,const char* text_button,EventHandler& handler,unsigned int id):
	m_handler(*this,handler),m_id(id)
	{
	m_box=BoxHorizontal::create(parent,&m_handler);
	m_box->slaveAssign(*this);
	m_box->insertModeSet(0);
	m_label=Label::create(*m_box,label);
	m_box->insertModeSet(BoxHorizontal::INSERTMODE_FILL|BoxHorizontal::INSERTMODE_EXPAND);
	m_textbox=Textbox::create(*m_box,EventHandlerInternal::COMMAND_TEXTCHANGED);
	m_box->insertModeSet(BoxHorizontal::INSERTMODE_END);
	m_button=Button::create(*m_box,text_button,EventHandlerInternal::COMMAND_ACTION);
	}

InputEntry::~InputEntry()
	{
	m_button->destroy();
	m_textbox->destroy();
	m_label->destroy();
	m_box->slaveRelease();
	}

const GuiHandle& InputEntry::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

const char* InputEntry::textGet() const
	{return m_textbox->textGet();}

void InputEntry::textSet(const char* text)
	{m_textbox->textSet(text);}
