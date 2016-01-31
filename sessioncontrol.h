#ifdef __WAND__
target[name[sessioncontrol.h] type[include]]
dependency[sessioncontrol.o]
#endif

#ifndef SESSIONCONTROL_H
#define SESSIONCONTROL_H

#include "framework/widget.h"
#include "framework/button.h"
#include "framework/textbox.h"

class Session;
class SessionView;
class GuiContainer;
class BoxVertical;

class SessionControl:public Widget
	{
	public:
		static SessionControl* create(GuiContainer& parent,Session& session
			,SessionView& view);
		void destroy();
		const GuiHandle& handleNativeGet() const;

		void doTitleChange(const char* title);
		void doSessionNew();
		void doSessionLoad();
		void doSessionSave();
		void doSessionSaveAs();
		void doEngineConnect();
		void doFullscreen();

	private:
		Session* r_session;
		SessionView* r_view;
		BoxVertical* m_box;
		Textbox* m_session_title;
		Button* m_session_new;
		Button* m_session_load;
		Button* m_session_save;
		Button* m_session_saveas;
		Button* m_engine_connect;
		Button* m_fullscreen;

		class ActionHandler:public Button::EventHandler,public Textbox::EventHandler
			{
			public:
				ActionHandler(SessionControl& ctrl);
				void onActionPerform(Button& source);
				void onLeave(Textbox& source);

			private:
				SessionControl& r_ctrl;
			} m_handler;

		SessionControl(GuiContainer& parent,Session& session,SessionView& view);
		~SessionControl();

	};

#endif
