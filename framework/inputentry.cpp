#ifdef __WAND__
target[name[inputentry.o] type[object]]
#endif

#include "inputentry.h"
#include "button.h"
#include "textbox.h"
#include "label.h"
#include "boxhorizontal.h"

#include <cstdio>

InputEntry::EventHandlerInternal::EventHandlerInternal(InputEntry& object):
	r_object(object)
	{}

void InputEntry::EventHandlerInternal::onActionPerform(Button& source)
	{
	r_object.doButtonClick();
	}

void InputEntry::EventHandlerInternal::onLeave(Textbox& source)
	{
	r_object.doTextChanged();
	}

InputEntry::EventHandler InputEntry::s_default_handler;

InputEntry::InputEntry(GuiContainer& parent,EventHandler& handler
	,unsigned int id,const char* label,const char* text_button):
	r_handler(&handler),m_handler(*this),m_id(id)
	{
	m_box=BoxHorizontal::create(parent);
	m_box->slaveAssign(*this);
	m_box->insertModeSet(0);
	m_label=Label::create(*m_box,label);
	m_box->insertModeSet(BoxHorizontal::INSERTMODE_FILL|BoxHorizontal::INSERTMODE_EXPAND);
	m_textbox=Textbox::create(*m_box,m_handler,0);
	m_box->insertModeSet(BoxHorizontal::INSERTMODE_END);
	m_button=Button::create(*m_box,m_handler,0,text_button);
	}

InputEntry::~InputEntry()
	{
	m_button->destroy();
	m_textbox->destroy();
	m_label->destroy();
	m_box->slaveRelease();
	m_box->destroy();
	}

const GuiHandle& InputEntry::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

const char* InputEntry::textGet() const
	{return m_textbox->textGet();}

void InputEntry::textSet(const char* text)
	{m_textbox->textSet(text);}
