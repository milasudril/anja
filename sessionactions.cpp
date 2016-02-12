#ifdef __WAND__
target[name[sessionactions.o] type[object]]
#endif

#include "sessionactions.h"
#include "session.h"
#include "framework/boxvertical.h"
#include "framework/filenamepicker.h"
#include "framework/textbox.h"
#include "framework/delimiter.h"

static constexpr unsigned int SESSION_NEW=0;
static constexpr unsigned int SESSION_LOAD=1;
static constexpr unsigned int SESSION_SAVE=2;
static constexpr unsigned int SESSION_SAVEAS=3;
static constexpr unsigned int ENGINE_START=4;
static constexpr unsigned int ENGINE_STOP=5;
static constexpr unsigned int FULLSCREEN=6;
static constexpr unsigned int EXIT=7;


SessionActions::ActionHandler::ActionHandler(SessionActions& ctrl):r_ctrl(ctrl)
	{}

void SessionActions::ActionHandler::onActionPerform(Button& source)
	{
	switch(source.idGet())
		{
		case SESSION_NEW:
			r_ctrl.doSessionNew();
			break;
		case SESSION_LOAD:
			r_ctrl.doSessionLoad();
			break;
		case SESSION_SAVE:
			r_ctrl.doSessionSave();
			break;
		case SESSION_SAVEAS:
			r_ctrl.doSessionSaveAs();
			break;
		case ENGINE_START:
			r_ctrl.doEngineStart();
			break;
		case ENGINE_STOP:
			r_ctrl.doEngineStop();
			break;
		case FULLSCREEN:
			r_ctrl.doFullscreen();
			break;
		case EXIT:
			r_ctrl.doExit();
			break;
		}
	}


SessionActions* SessionActions::create(GuiContainer& parent
	,EventHandler& handler,Session& session)
	{return new SessionActions(parent,handler,session);}

SessionActions::SessionActions(GuiContainer& parent,EventHandler& handler
	,Session& session):r_session(&session),r_handler(&handler),m_handler(*this)
	{
	m_box=BoxVertical::create(parent);
	m_box->slaveAssign(*this);
	m_session_new=Button::create(*m_box,m_handler,SESSION_NEW,"New session");
	m_session_load=Button::create(*m_box,m_handler,SESSION_LOAD,"Load session");
	m_session_save=Button::create(*m_box,m_handler,SESSION_SAVE,"Save session");
	m_session_saveas=Button::create(*m_box,m_handler,SESSION_SAVEAS,"Save session as");
	m_delimiter_a=Delimiter::create(*m_box);
	m_engine_start=Button::create(*m_box,m_handler,ENGINE_START,"Start engine");
	m_engine_stop=Button::create(*m_box,m_handler,ENGINE_STOP,"Stop engine");
	m_delimiter_b=Delimiter::create(*m_box);
	m_fullscreen=Button::create(*m_box,m_handler,FULLSCREEN,"Fullscreen");
	m_exit=Button::create(*m_box,m_handler,EXIT,"Exit");
	}

SessionActions::~SessionActions()
	{
	m_exit->destroy();
	m_fullscreen->destroy();
	m_delimiter_b->destroy();
	m_engine_stop->destroy();
	m_engine_start->destroy();
	m_delimiter_a->destroy();
	m_session_saveas->destroy();
	m_session_save->destroy();
	m_session_load->destroy();
	m_session_new->destroy();
	m_box->slaveRelease();
	m_box->destroy();
	}

void SessionActions::destroy()
	{
	delete this;
	}

const GuiHandle& SessionActions::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

void SessionActions::statusUpdate()
	{
	if(r_session->connectedIs())
		{m_engine_start->titleSet("Restart engine");}
	else
		{m_engine_start->titleSet("Start engine");}
	}

void SessionActions::fullscreenSet()
	{
	m_fullscreen->titleSet("Windowed");
	}

void SessionActions::fullscreenUnset()
	{
	m_fullscreen->titleSet("Fullscreen");
	}

void SessionActions::doSessionSaveAs()
	{
	auto filename_suggested=r_session->titleGet();
	filename_suggested.append(".txt").append('\0');
	auto picker=FilenamePicker::create(*this
		,filename_suggested.begin()
		,FilenamePicker::MODE_SAVE);

	auto filename=picker->filenameGet();

	if(filename!=nullptr)
		{
		r_handler->onSessionSaveAs(*this,filename);
		}
	}

void SessionActions::doSessionSave()
	{
	auto filename=r_session->filenameGet().begin();
	if(*filename=='\0')
		{
		doSessionSaveAs();
		}
	else
		{r_handler->onSessionSaveAs(*this,filename);}
	}

void SessionActions::doSessionLoad()
	{
	auto picker=FilenamePicker::create(*this,r_session->filenameGet().begin()
		,FilenamePicker::MODE_OPEN);

	auto filename=picker->filenameGet();

	if(filename!=nullptr)
		{r_handler->onSessionLoad(*this,filename);}
	}

void SessionActions::askSave()
	{
	if(r_session->dirtyIs())
		{
		auto answer=notifyUser("Do you want to save changes to this session?"
			,"Close Anja session",MESSAGE_TYPE_WARNING|MESSAGE_ANSWERS_YESNO);
		if(answer==MESSAGE_ANSWER_YES)
			{doSessionSave();}
		}
	}
