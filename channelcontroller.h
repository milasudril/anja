#ifdef __WAND__
target[name[channelcontroller.h] type[include]]
dependency[channelcontroller.o]
#endif

#include "channelstrip.h"
#ifndef CHANNELCONTROLLER_H
#define CHANNELCONTROLLER_H

class ChannelData;
class Session;
class SessionView;

class ChannelController:public ChannelStrip::EventHandler
	{
	public:
		ChannelController(Session& session):r_session(&session){}

		void onLabelChange(ChannelStrip& source,const char* label);
		void onFadeTimeChange(ChannelStrip& source,float time);
		void onGainChange(ChannelStrip& source,float value);
		void onColorChange(ChannelStrip& source,const ColorRGBA& color);

		void sessionViewSet(SessionView* view)
			{r_view=view;}

	private:
		Session* r_session;
		SessionView* r_view;
	};

#endif
