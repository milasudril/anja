#ifdef __WAND__
target[name[sessioncontrol.o] type[object]]
#endif

#include "sessioncontrol.h"
#include "session.h"
#include "sessionview.h"
#include "framework/boxvertical.h"
#include "framework/filenamepicker.h"
#include "framework/textbox.h"

static constexpr unsigned int SESSION_NEW=0;
static constexpr unsigned int SESSION_LOAD=1;
static constexpr unsigned int SESSION_SAVE=2;
static constexpr unsigned int SESSION_SAVEAS=3;
static constexpr unsigned int ENGINE_CONNECT=4;
static constexpr unsigned int ENGINE_DISCONNECT=5;
static constexpr unsigned int FULLSCREEN=6;


SessionControl::ActionHandler::ActionHandler(SessionControl& ctrl):r_ctrl(ctrl)
	{}

void SessionControl::ActionHandler::onActionPerform(Button& source)
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
		case ENGINE_CONNECT:
			r_ctrl.doEngineConnect();
			break;
		case FULLSCREEN:
			r_ctrl.doFullscreen();
			break;
		}
	}

void SessionControl::ActionHandler::onLeave(Textbox& source)
	{
	r_ctrl.doTitleChange(source.textGet());
	}



SessionControl* SessionControl::create(GuiContainer& parent,Session& session
	,SessionView& view)
	{return new SessionControl(parent,session,view);}

SessionControl::SessionControl(GuiContainer& parent,Session& session
	,SessionView& view):
	r_session(&session),r_view(&view),m_handler(*this)
	{
	m_box=BoxVertical::create(parent);
	m_box->slaveAssign(*this);

	m_session_title=Textbox::create(*m_box,m_handler,0);
	m_session_new=Button::create(*m_box,m_handler,SESSION_NEW,"New session");
	m_session_load=Button::create(*m_box,m_handler,SESSION_LOAD,"Load session");
	m_session_save=Button::create(*m_box,m_handler,SESSION_SAVE,"Save session");
	m_session_saveas=Button::create(*m_box,m_handler,SESSION_SAVEAS,"Save session as");
	m_engine_connect=Button::create(*m_box,m_handler,ENGINE_CONNECT,"Connect engine");
	m_fullscreen=Button::create(*m_box,m_handler,FULLSCREEN,"Fullscreen");
	m_session_title->textSet(session.titleGet().begin());
	}

SessionControl::~SessionControl()
	{
	m_fullscreen->destroy();
	m_engine_connect->destroy();
	m_session_saveas->destroy();
	m_session_save->destroy();
	m_session_load->destroy();
	m_session_new->destroy();
	m_session_title->destroy();
	m_box->slaveRelease();
	}

void SessionControl::destroy()
	{
	delete this;
	}

const GuiHandle& SessionControl::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

void SessionControl::doSessionNew()
	{
	bool status=r_session->connectedIs();
	r_session->audioServerDisconnect();
	r_session->clear();
	r_view->sessionSet(*r_session);
	if(status)
		{r_session->audioServerConnect();}
	m_session_title->textSet(r_session->titleGet().begin());
	}

void SessionControl::doSessionLoad()
	{
	bool status=r_session->connectedIs();

	auto picker=FilenamePicker::create(*this,r_session->filenameGet().begin()
		,FilenamePicker::MODE_OPEN);

	auto res=picker->filenameGet();

	if(res!=nullptr)
		{
		r_session->load(res);
		r_view->sessionSet(*r_session);
		}

	if(status)
		{r_session->audioServerConnect();}

	m_session_title->textSet(r_session->titleGet().begin());
	}

void SessionControl::doSessionSave()
	{
	if(*(r_session->filenameGet().begin())=='\0')
		{
		doSessionSaveAs();
		return;
		}

	r_session->save();
	}

void SessionControl::doSessionSaveAs()
	{
	auto filename_suggested=r_session->titleGet();
	filename_suggested.append(".txt").append('\0');
	auto picker=FilenamePicker::create(*this
		,filename_suggested.begin()
		,FilenamePicker::MODE_SAVE);

	auto res=picker->filenameGet();

	if(res!=nullptr)
		{
		r_session->save(res);
		r_view->sessionSet(*r_session);
		}
	}

void SessionControl::doEngineConnect()
	{
	if(r_session->connectedIs())
		{
		r_session->audioServerDisconnect();
		m_engine_connect->titleSet("Connect engine");
		}
	else
		{
		r_session->audioServerConnect();
		m_engine_connect->titleSet("Disconnect engine");
		}
	}

void SessionControl::doFullscreen()
	{
	r_view->fullscreenToggle();
	}

void SessionControl::doTitleChange(const char* title)
	{
	r_session->titleSet(title);
	r_view->sessionSet(*r_session);
	}
