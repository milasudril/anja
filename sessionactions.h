#ifdef __WAND__
target[name[sessionactions.h] type[include]]
dependency[sessionactions.o]
#endif

#ifndef SESSIONACTIONS_H
#define SESSIONACTIONS_H

#include "framework/widget.h"
#include "framework/button.h"

class Session;
class GuiContainer;
class BoxVertical;
class Delimiter;

class SessionActions:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onSessionNew(SessionActions& source)=0;
				virtual void onSessionLoad(SessionActions& source,const char* filename)=0;
				virtual void onSessionSaveAs(SessionActions& source,const char* filename)=0;
				virtual void onEngineStart(SessionActions& source)=0;
				virtual void onEngineStop(SessionActions& source)=0;
				virtual void onFullscreen(SessionActions& source)=0;
				virtual void onExit(SessionActions& source)=0;
			};

		static SessionActions* create(GuiContainer& parent
			,EventHandler& handler,Session& session);
		void destroy();
		const GuiHandle& handleNativeGet() const;

		void doSessionNew()
			{r_handler->onSessionNew(*this);}

		void doSessionLoad();

		void doSessionSave();

		void doSessionSaveAs();

		void doEngineStart()
			{r_handler->onEngineStart(*this);}

		void doEngineStop()
			{r_handler->onEngineStop(*this);}

		void doFullscreen()
			{r_handler->onFullscreen(*this);}

		void doExit()
			{r_handler->onExit(*this);}

		Session& sessionGet()
			{return *r_session;}

		void statusUpdate();
		void fullscreenSet();
		void fullscreenUnset();
		void askSave();

	private:
		Session* r_session;
		EventHandler* r_handler;
		BoxVertical* m_box;
		Button* m_session_new;
		Button* m_session_load;
		Button* m_session_save;
		Button* m_session_saveas;
		Delimiter* m_delimiter_a;
		Button* m_engine_start;
		Button* m_engine_stop;
		Delimiter* m_delimiter_b;
		Button* m_fullscreen;
		Button* m_exit;

		class ActionHandler:public Button::EventHandler
			{
			public:
				ActionHandler(SessionActions& ctrl);
				void onActionPerform(Button& source);

			private:
				SessionActions& r_ctrl;
			} m_handler;

		SessionActions(GuiContainer& parent,EventHandler& handler
			,Session& session);
		~SessionActions();

	};

#endif
