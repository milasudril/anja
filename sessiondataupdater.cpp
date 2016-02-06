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
	r_view->statusUpdate();
	}

void SessionDataUpdater::onDescriptionChange(SessionDataView& source
	,const char* description)
	{
	auto& session=source.sessionGet();
	session.descriptionSet(description);
	r_view->statusUpdate();
	}


void SessionDataUpdater::onOptionSet(SessionDataView& source,unsigned int options)
	{
	auto& session=source.sessionGet();
	session.flagSet(options);
	r_view->statusUpdate();
	}


void SessionDataUpdater::onOptionUnset(SessionDataView& source,unsigned int options)
	{
	auto& session=source.sessionGet();
	session.flagUnset(options);
	r_view->statusUpdate();
	}
