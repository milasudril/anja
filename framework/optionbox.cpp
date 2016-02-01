#ifdef __WAND__
target[name[optionbox.o] type[object]]
#endif

#include "optionbox.h"
#include "boxvertical.h"
#include "label.h"

OptionBox::EventHandler OptionBox::s_default_handler;

OptionBox::EventHandlerCheckbox::EventHandlerCheckbox(OptionBox& box):r_box(box)
	{}

void OptionBox::EventHandlerCheckbox::onSet(Checkbox& source)
	{
	r_box.doSet(source.idGet());
	}

void OptionBox::EventHandlerCheckbox::onUnset(Checkbox& source)
	{
	r_box.doUnset(source.idGet());
	}

OptionBox::OptionBox(GuiContainer& parent,EventHandler& handler,unsigned int id
	,const char* title,const char* const* options):
	r_handler(&handler),m_handler_cbox(*this)
	{
	m_box=BoxVertical::create(parent);
	m_box->slaveAssign(*this);
	m_box->insertModeSet(BoxVertical::INSERTMODE_LEFT);
	m_label=Label::create(*m_box,title);
	unsigned int i=0;
	while(*options!=nullptr)
		{
		m_options.append(Checkbox::create(*m_box,m_handler_cbox,i,*options));
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
	m_box->destroy();
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
