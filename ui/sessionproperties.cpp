//@	{"targets":[{"name":"sessionproperties.o","type":"object"}]}

#include "sessionproperties.hpp"
#include "../sessiondata/session.hpp"

using namespace Anja;

void SessionProperties::changed(TextEntry& entry,TextEntryId id)
	{
	switch(id)
		{
		case TextEntryId::TITLE:
			r_session->titleSet(entry.content());
			//TODO Notify owner
			break;
		}
	}

void SessionProperties::changed(SourceView& entry,SourceViewId id)
	{
	switch(id)
		{
		case SourceViewId::DESCRIPTION:
			r_session->descriptionSet(entry.content());
			//TODO Notify owner
			break;
		}
	}

void SessionProperties::clicked(OptionList& options,OptionListId id,Checkbox& option)
	{
	switch(id)
		{
		case OptionListId::OPTIONS:
			if(option.state())
				{r_session->flagSet(option.id());}
			else
				{r_session->flagsUnset(option.id());}
			//TODO Notify owner
			break;
		}
	}

void SessionProperties::sessionUpdated()
	{
	m_title_input.content(r_session->titleGet().begin());
	m_description_input.content(r_session->descriptionGet().begin());
	m_options_input.selected(r_session->flagsGet());
	}

SessionProperties::SessionProperties(Container& cnt,Session& session):
	 r_session(&session)
	,m_box(cnt,true)
		,m_title(m_box.insertMode({1,0}),false)
			,m_title_label(m_title.insertMode({2,0}),"Title:")
			,m_title_input(m_title.insertMode({2,Box::EXPAND|Box::FILL}))
		,m_description(m_box,false)
			,m_description_label(m_description.insertMode({2,0}),"Description:")
		,m_description_input(m_box.insertMode({0,Box::EXPAND|Box::FILL}))
		,m_options(m_box.insertMode({1,0}),false)
			,m_options_label(m_options.insertMode({2,0}),"Options:")
		,m_options_input(m_box,true)
	{
	m_options_input.append(session.flagNames());
	m_description_input.wordwrap(1);
	m_title_input.callback(*this,TextEntryId::TITLE);
	m_description_input.callback(*this,SourceViewId::DESCRIPTION);
	m_options_input.callback(*this,OptionListId::OPTIONS);
	sessionUpdated();
	}
