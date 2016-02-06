#ifdef __WAND__
target[name[sessioncontrol.h] type[include]]
dependency[sessioncontrol.o]
#endif

#ifndef SESSIONCONTROL_H
#define SESSIONCONTROL_H

#include "framework/widget.h"
#include "framework/button.h"

class Session;
class SessionView;
class GuiContainer;
class BoxVertical;
class Delimiter;

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
		void doEngineStart();
		void doEngineStop();
		void doFullscreen();

	private:
		Session* r_session;
		SessionView* r_view;
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

		class ActionHandler:public Button::EventHandler
			{
			public:
				ActionHandler(SessionControl& ctrl);
				void onActionPerform(Button& source);

			private:
				SessionControl& r_ctrl;
			} m_handler;

		SessionControl(GuiContainer& parent,Session& session,SessionView& view);
		~SessionControl();

	};

#endif
