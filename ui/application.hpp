//@	{
//@	"targets":[{"name":"application.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"application.o","rel":"implementation"}]
//@	}

#ifndef ANJA_APPLICATION_HPP
#define ANJA_APPLICATION_HPP

#include "uicontext.hpp"
#include "window.hpp"
#include "buttonlist.hpp"
#include "sessionpropertieseditor.hpp"
#include "sessioneditor.hpp"
#include "../sessiondata/session.hpp"

namespace Anja
	{
	class Application
		{
		public:
			Application();

			void closing(Window& win,int id);

			Application& run()
				{
				m_ctx.run(*this);
				return *this;
				}

			Application& sessionLoad(const char* filename);

			UiContext::RunStatus idle(UiContext& ctx);

			void keyDown(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id);
			void keyUp(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id);
			void clicked(ButtonList& buttons,int id,Button& btn);

			void titleChanged(SessionPropertiesEditor& editor,int id);
			void descriptionChanged(SessionPropertiesEditor& editor,int id);
			void optionChanged(SessionPropertiesEditor& editor,int id,int option);


		private:
			UiContext m_ctx;
			Session m_session;
			Window m_mainwin;
				Box m_cols;
					Anja::ButtonList m_session_control;
					Anja::SessionEditor m_session_editor;
			bool m_fullscreen;
		};
	}

#endif
