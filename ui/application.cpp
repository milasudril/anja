//@	{
//@	"targets":[{"name":"application.o","type":"object"}]
//@	}

#include "application.hpp"

using namespace Anja;

static void title_update(const Session& session,Window& win)
	{
	String title(session.titleGet());
	title.append("—Anja");
	win.title(title.begin());
	}

void Application::closing(Window& win,int id)
	{
	//TODO: Ask for saving data
	m_ctx.exit();
	}

UiContext::RunStatus Application::idle(UiContext& ctx)
	{
	return UiContext::RunStatus::WAIT;
	}

void Application::keyDown(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id)
	{
	printf("%d down\n",scancode);
	}

void Application::keyUp(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id)
	{
	printf("%d up\n",scancode);
	}

void Application::clicked(ButtonList& buttons,int id,Button& btn)
	{
	switch(btn.id())
		{
		case 0:
			//TODO: Check if session is dirty first
			m_session.clear();
			m_session_editor.sessionUpdated();
			title_update(m_session,m_mainwin);
			break;
		case 2:
			m_session.save(m_session.filenameGet().begin());
			break;
		case 6:
			m_fullscreen=!m_fullscreen;
			m_mainwin.fullscreen(m_fullscreen);
			btn.label(m_fullscreen?"Windowed":"Fullscreen");
			break;
		case 7:
			//TODO: Ask for saving data
			m_ctx.exit();
			break;
		}
	btn.state(0);
	}

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



Application::Application():
	m_mainwin("New session--Anja")
		,m_cols(m_mainwin,false)
			,m_session_control(m_cols,true)
			,m_session_editor(m_cols.insertMode({2,Anja::Box::EXPAND|Anja::Box::FILL}),m_session)
	,m_fullscreen(0)
	{
	m_session_control.append("New session","Load session","Save session"
		,"Save session as","","Start engine","Stop engine","","Fullscreen"
		,"Exit","About Anja");
	m_session_control.callback(*this,0);
	m_mainwin.callback(*this,0);
	m_session_editor.sessionChangedCallback(*this,0);
	title_update(m_session,m_mainwin);
	m_mainwin.show();
	}

