#ifdef __WAND__
target[name[optionbox.o] type[object]]
#endif

#include "optionbox.h"
#include "checkbox.h"
#include "label.h"

OptionBox::EventHandler OptionBox::s_default_handler;

void OptionBox::EventHandlerCheckbox::onCommand(BoxVertical& source,unsigned int command_id)
	{
	if(r_box.stateGet(command_id))
		{r_handler.onSet(r_box,command_id);}
	else
		{r_handler.onUnset(r_box,command_id);}
	}

OptionBox::OptionBox(GuiContainer& parent,EventHandler& handler
	,const char* title,const char* const* options):m_handler_cbox(*this,handler)
	{
	m_box=BoxVertical::create(parent,&m_handler_cbox);
	m_box->slaveAssign(*this);
	m_box->insertModeSet(BoxVertical::INSERTMODE_LEFT);
	m_label=Label::create(*m_box,title);
	unsigned int i=0;
	while(*options!=nullptr)
		{
		m_options.append(Checkbox::create(*m_box,*options,i));
		++options;
		++i;
		}
	}

OptionBox::~OptionBox()
	{
	auto ptr=m_options.end();
	auto ptr_begin=m_options.begin();
	while(ptr!=ptr_begin)
		{
		--ptr;
		(*ptr)->destroy();
		}
	m_label->destroy();
	m_box->slaveRelease();
	}

const GuiHandle& OptionBox::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

bool OptionBox::stateGet(unsigned int option) const
	{
	return m_options[option]->stateGet();
	}

void OptionBox::stateSet(unsigned int option,bool state_new)
	{
	return m_options[option]->stateSet(state_new);
	}
