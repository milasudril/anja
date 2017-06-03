//@	{
//@	"targets":
//@		[{
//@		"name":"application.o","type":"object"
//@		,"dependencies":[{"ref":"../logo.png","rel":"generated"}]
//@		,"cxxoptions":{"cflags_extra":["fno-lto"]}
//@		,"include_targets":["../projectinfo.hpp"]
//@		}]
//@	}

#include "application.hpp"
#include "filenameselect.hpp"
#include "../common/blob.hpp"
#include "../sessiondata/keymap.hpp"
#include <maike/targetinclude.hpp>

using namespace Anja;

ANJA_BLOB(uint8_t,s_logo,MAIKE_TARGET(../logo.png));

static void title_update(const Session& session,Window& win)
	{
	String title(session.titleGet());
	title.append("—Anja");
	win.title(title.begin());
	}

void Application::save_ask(ConfirmSaveDialogId id)
	{
	const char* title="";
	switch(id)
		{
		case ConfirmSaveDialogId::EXIT:
			title="Anja: Closing Anja";
			break;
		case ConfirmSaveDialogId::SESSION_LOAD:
			title="Anja: Loading another session";
			break;
		case ConfirmSaveDialogId::SESSION_RELOAD:
			title="Anja: Reloading session";
			break;
		case ConfirmSaveDialogId::SESSION_NEW:
			title="Anja: Creating a new session";
			break;
		}
	String msg("Do you want to save changes to ");
	msg.append(m_session.titleGet()).append("?");
	m_confirm.reset(new Dialog<Message,ConfirmSaveDialog>(m_mainwin,title,msg.begin()
		,Message::Type::WARNING));
	m_confirm->callback(*this,id);
	}

void Application::dismiss(Dialog<Message,ConfirmSaveDialog>& dlg,ConfirmSaveDialogId id)
	{
	m_confirm.reset();
	}

void Application::confirmPositive(Dialog<Message,ConfirmSaveDialog>& dlg,ConfirmSaveDialogId id)
	{
	if(sessionSave())
		{confirmNegative(dlg,id);}
	}

void Application::confirmNegative(Dialog<Message,ConfirmSaveDialog>& dlg,ConfirmSaveDialogId id)
	{
	m_confirm.reset();
	switch(id)
		{
		case ConfirmSaveDialogId::EXIT:
			m_ctx.exit();
			break;
		case ConfirmSaveDialogId::SESSION_LOAD:
			sessionLoad();
			break;
		case ConfirmSaveDialogId::SESSION_RELOAD:
			sessionLoad(m_session.filenameGet().begin());
			break;
		case ConfirmSaveDialogId::SESSION_NEW:
			sessionNew();
			break;
		}
	}

Application& Application::sessionNew()
	{
	m_session.clear();
	m_session_editor.sessionUpdated();
	title_update(m_session,m_mainwin);
	return *this;
	}

bool Application::sessionSave()
	{
	if(m_session.filenameGet().length()!=0)
		{
		m_session.save(m_session.filenameGet().begin());
		return 1;
		}
	return sessionSaveAs();
	}

bool Application::sessionSaveAs()
	{
	auto name=std::string(m_session.filenameGet().begin());
	if(filenameSelect(m_mainwin,m_session.directoryGet().begin()
		,name,Anja::FilenameSelectMode::SAVE))
		{
		m_session.save(name.c_str());
		m_session_editor.sessionUpdated();
		return 1;
		}
	return 0;
	}

Application& Application::sessionLoad()
	{
	auto name=std::string(m_session.filenameGet().begin());
	if(filenameSelect(m_mainwin,m_session.directoryGet().begin()
		,name,Anja::FilenameSelectMode::OPEN,[this](const char* name)
			{return m_session.loadPossible(name);},"Anja session files"))
		{sessionLoad(name.c_str());}
	return *this;
	}



void Application::closing(Window& win,int id)
	{
	if(m_session.dirtyIs())
		{save_ask(ConfirmSaveDialogId::EXIT);}
	else
		{m_ctx.exit();}
	}

UiContext::RunStatus Application::idle(UiContext& ctx)
	{
	return UiContext::RunStatus::WAIT;
	}

void Application::keyDown(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id)
	{
	if(m_engine /*&& !m_keystate[scancode]*/)
		{
		auto note=scancodeToMIDI(scancode);
		if(note!=0xff)
			{m_engine->messagePost(MIDI::Message{MIDI::StatusCodes::NOTE_ON,0,note,127});}
		else
			{
			if(scancode==57)
				{
			//Play audition...
				}
			}
		}
	m_keystate[scancode]=1;
	}

void Application::keyUp(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id)
	{
	if(m_engine)
		{
		auto note=scancodeToMIDI(scancode);
		if(note!=0xff)
			{m_engine->messagePost(MIDI::Message{MIDI::StatusCodes::NOTE_OFF,0,note,127});}
		}
	m_keystate[scancode]=0;
	}


void Application::clicked(ButtonList& buttons,int id,Button& btn)
	{
	try
		{
		switch(btn.id())
			{
			case 0:
				if(m_session.dirtyIs())
					{save_ask(ConfirmSaveDialogId::SESSION_NEW);}
				else
					{sessionNew();}
				break;
			case 1:
				if(m_session.dirtyIs())
					{save_ask(ConfirmSaveDialogId::SESSION_LOAD);}				else
					{sessionLoad();}
				break;
			case 2:
				if(m_session.dirtyIs())
					{save_ask(ConfirmSaveDialogId::SESSION_RELOAD);}
				else
					{sessionLoad(m_session.filenameGet().begin());}
				break;
			case 3:
				sessionSave();
				break;
			case 4:
				sessionSaveAs();
				break;
			case 5:
				m_engine.reset(new Engine(m_session));
				break;
			case 6:
				m_engine.reset();
				break;
			case 7:
				m_fullscreen=!m_fullscreen;
				m_mainwin.fullscreen(m_fullscreen);
				btn.label(m_fullscreen?"Windowed":"Fullscreen");
				break;
			case 8:
				if(m_session.dirtyIs())
					{save_ask(ConfirmSaveDialogId::EXIT);}
				else
					{m_ctx.exit();}
				break;
			case 9:
				m_about.reset(new Dialog<AboutBox,AboutDialog>(m_mainwin,"About Anja",ProjectInfo{}));
				m_about->widget().logo(s_logo_begin,s_logo_end);
				m_about->callback(*this,0);
				break;
			}
		}
	catch(const Error& e)
		{
		m_error.reset(new Dialog<Message,DialogOk>(m_mainwin,"Anja error",e.message(),Message::Type::ERROR));
		m_error->callback(*this,0);
		}
	btn.state(0);
	}

void Application::confirmPositive(Dialog<Message,DialogOk>& dlg,int id)
	{m_error.reset();}

void Application::confirmPositive(Dialog<AboutBox,AboutDialog>& dlg,int id)
	{m_about.reset();}

void Application::user1(Dialog<AboutBox,AboutDialog>& dlg,int id)
	{dlg.widget().legalBrief();}

void Application::user2(Dialog<AboutBox,AboutDialog>& dlg,int id)
	{dlg.widget().techstring();}

void Application::titleChanged(SessionPropertiesEditor& editor,int id)
	{
	title_update(m_session,m_mainwin);
	}

void Application::descriptionChanged(SessionPropertiesEditor& editor,int id)
	{}

void Application::optionChanged(SessionPropertiesEditor& editor,int id,int option)
	{}


Application& Application::sessionLoad(const char* filename)
	{
	m_session.load(filename);
	m_session_editor.sessionUpdated();
	title_update(m_session,m_mainwin);
	return *this;
	}

void Application::nameChanged(ChannelStrip& strip,int id)
	{
	if(m_engine && m_session.flagsGet()&Session::MULTIOUTPUT)
		{m_engine->waveOutName(id,strip.name().begin());}
	}

Application::Application():
	m_mainwin("New session--Anja")
		,m_cols(m_mainwin,false)
			,m_session_control(m_cols,true)
			,m_session_editor(m_cols.insertMode({2,Anja::Box::EXPAND|Anja::Box::FILL}),m_session)
	,m_fullscreen(0)
	{
	m_ctx.dark(1);
	m_session_control.append("New session","Load session","Reload session","Save session"
		,"Save session as","","Start engine","Stop engine","","Fullscreen"
		,"Exit","About Anja");
	m_session_control.callback(*this,0);
	m_mainwin.callback(*this,0);
	m_session_editor.callback(*this,0);
	title_update(m_session,m_mainwin);
	m_mainwin.show();
	}

