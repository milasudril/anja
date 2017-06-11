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
#include "dialog.hpp"
#include "message.hpp"
#include "aboutbox.hpp"
#include "../sessiondata/session.hpp"
#include "../engine/engine.hpp"
#include <memory>
#include <bitset>

namespace Anja
	{
	class Application
		{
		private:
			struct ConfirmSaveDialog
				{
				static constexpr const char* dismiss() noexcept
					{return "Cancel";}

				static constexpr const char* confirmPositive() noexcept
					{return "Save";}

				static constexpr const char* confirmNegative() noexcept
					{return "Don't save";}

				static constexpr const char* user1() noexcept
					{return nullptr;}

				static constexpr const char* user2() noexcept
					{return nullptr;}
				};

			struct AboutDialog
				{
				static constexpr const char* dismiss() noexcept
					{return nullptr;}

				static constexpr const char* confirmPositive() noexcept
					{return "OK";}

				static constexpr const char* confirmNegative() noexcept
					{return nullptr;}

				static constexpr const char* user1() noexcept
					{return "Legal info…";}

				static constexpr const char* user2() noexcept
					{return "Tech info…";}
				};

		public:
			Application();

			void closing(Window& win,int id);

			Application& run()
				{
				m_ctx.run(*this);
				return *this;
				}

			Application& sessionNew();
			Application& sessionLoad(const char* filename);
			Application& sessionLoad();
			bool sessionSave();
			bool sessionSaveAs();

			UiContext::RunStatus idle(UiContext& ctx);

			void keyDown(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id);
			void keyUp(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id);
			void clicked(ButtonList& buttons,int id,Button& btn);

			void titleChanged(SessionPropertiesEditor& editor,int id);
			void descriptionChanged(SessionPropertiesEditor& editor,int id);
			void optionChanged(SessionPropertiesEditor& editor,int id,int option);
			void nameChanged(ChannelStrip& strip,int id);
			void gainChanged(ChannelStrip& strip,int id);

			enum class ConfirmSaveDialogId:int{SESSION_NEW,SESSION_LOAD,SESSION_RELOAD,EXIT};

			void dismiss(Dialog<Message,ConfirmSaveDialog>& dlg,ConfirmSaveDialogId id);
			void confirmPositive(Dialog<Message,ConfirmSaveDialog>& dlg,ConfirmSaveDialogId id);
			void confirmNegative(Dialog<Message,ConfirmSaveDialog>& dlg,ConfirmSaveDialogId id);
			void confirmPositive(Dialog<AboutBox,AboutDialog>& dlg,int id);
			void confirmPositive(Dialog<Message,DialogOk>& dlg,int id);
			void user1(Dialog<AboutBox,AboutDialog>& dlg,int id);
			void user2(Dialog<AboutBox,AboutDialog>& dlg,int id);


		private:
			UiContext m_ctx;
			Session m_session;
			Window m_mainwin;
				Box m_cols;
					Anja::ButtonList m_session_control;
					Box m_rows;
						Message m_status;
						Separator m_sep;
						Message m_ch_status;
						Anja::SessionEditor m_session_editor;
			bool m_fullscreen;

			std::unique_ptr<Dialog<Message,ConfirmSaveDialog> > m_confirm;
			void save_ask(ConfirmSaveDialogId id);

			std::unique_ptr<Dialog<AboutBox,AboutDialog>> m_about;
			std::unique_ptr<Dialog<Message,DialogOk> > m_error;

			std::unique_ptr<Engine> m_engine;
			std::bitset<256> m_keystate;

			void engine_stop();
			void engine_start();
		};
	}

#endif
