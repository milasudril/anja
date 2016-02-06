#ifdef __WAND__
target[name[sessiondataupdater.h] type[include]]
dependency[sessiondataupdater.o]
#endif

#ifndef SESSIONDATACONTROLLER_H
#define SESSIONDATACONTROLLER_H

#include "sessiondataview.h"
#include "sessionview.h"

class SessionDataUpdater:public SessionDataView::EventHandler
	{
	public:
		SessionDataUpdater():r_view(nullptr){}

		void onTitleChange(SessionDataView& source,const char* title);
		void onDescriptionChange(SessionDataView& source,const char* description);
		void onOptionSet(SessionDataView& source,unsigned int options);
		void onOptionUnset(SessionDataView& source,unsigned int options);

		void sessionViewSet(SessionView* view)
			{r_view=view;}

	private:
		SessionView* r_view;
	};

#endif
