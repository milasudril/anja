#ifdef __WAND__
target[name[sessiondataupdater.o] type[object]]
#endif

#include "sessiondataupdater.h"
#include "session.h"

void SessionDataUpdater::onTitleChange(SessionDataView& source,const char* title)
	{
	auto& session=source.sessionGet();
	session.titleSet(title);
	r_view->sessionTitleUpdate();

	if(session.restartNeeded())
		{
	//	TODO: Notify user that engine needs to be restarted;
		}

	}

void SessionDataUpdater::onDescriptionChange(SessionDataView& source
	,const char* description)
	{
	auto& session=source.sessionGet();
	session.descriptionSet(description);
	if(session.restartNeeded())
		{
	//	TODO: Notify user that engine needs to be restarted;
		}
	}


void SessionDataUpdater::onOptionSet(SessionDataView& source,unsigned int options)
	{
	auto& session=source.sessionGet();
	session.flagSet(options);
	if(session.restartNeeded())
		{
	//	TODO: Notify user that engine needs to be restarted;
		}
	}


void SessionDataUpdater::onOptionUnset(SessionDataView& source,unsigned int options)
	{
	auto& session=source.sessionGet();
	session.flagUnset(options);
	if(session.restartNeeded())
		{
	//	TODO: Notify user that engine needs to be restarted;
		}
	}
