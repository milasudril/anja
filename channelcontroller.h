#ifdef __WAND__
target[name[channelcontroller.h] type[include]]
dependency[channelcontroller.o]
#endif

#ifndef CHANNELCONTROLLER_H
#define CHANNELCONTROLLER_H

#include "channelstriphandler.h"

class Session;
class SessionView;

class ChannelController:public ChannelStripHandler::EventHandler
	{
	public:
		ChannelController(Session& session):r_session(&session){}

		void onLabelChange(unsigned int channel,const char* label);
		void onFadeTimeChange(unsigned int channel,float time);
		void onGainChange(unsigned int channel,float value);
		void onColorChange(unsigned int channel,const ColorRGBA& color);
		void onMasterGainChange(float value);

		void sessionViewSet(SessionView* view)
			{r_view=view;}

	private:
		Session* r_session;
		SessionView* r_view;
	};

#endif
