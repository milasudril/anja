#ifdef __WAND__
target[name[sessioncontrol.o] type[object]]
#endif

#include "sessioncontrol.h"
#include "sessionview.h"
#include "session.h"
#include "framework/window.h"

SessionControl::SessionControl(Window& window):r_window(&window)
	{}

void SessionControl::onSessionNew(SessionActions& source)
	{
	source.askSave();
	auto& session=source.sessionGet();

	bool status=session.connectedIs();
	session.audioServerDisconnect();
	session.clear();
	r_view->sessionSet(session);
	if(status)
		{session.audioServerConnect();}
	}

void SessionControl::onSessionLoad(SessionActions& source,const char* filename)
	{
	source.askSave();
	auto& session=source.sessionGet();
	bool status=session.connectedIs();
	session.load(filename);
	r_view->sessionSet(session);
	if(status)
		{session.audioServerConnect();}
	}

void SessionControl::onSessionSaveAs(SessionActions& source,const char* filename)
	{
	auto& session=source.sessionGet();

	session.save(filename);
	r_view->sessionSet(session);
	}

void SessionControl::onEngineStart(SessionActions& source)
	{
	auto& session=source.sessionGet();

	session.audioServerConnect();
	source.statusUpdate();
	r_view->statusUpdate();
	}

void SessionControl::onEngineStop(SessionActions& source)
	{
	auto& session=source.sessionGet();

	session.audioServerDisconnect();
	source.statusUpdate();
	r_view->statusUpdate();
	}

void SessionControl::onFullscreen(SessionActions& source)
	{
	r_view->fullscreenToggle();
	if(r_view->fullscreenIs())
		{source.fullscreenSet();}
	else
		{source.fullscreenUnset();}
	}

void SessionControl::onExit(SessionActions& source)
	{
	source.askSave();
	r_view->destroy();
	r_window->destroy();
	}
