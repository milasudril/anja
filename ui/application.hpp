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
#include "imagelist.hpp"
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
				m_cmd_ready.set();
				m_ctx.run(*this);
				return *this;
				}

			Application& sessionNew();
			Application& sessionLoad(const char* filename);
			Application& sessionLoad();
			Application& exit();
			bool sessionSave();
			bool sessionSaveAs();

			Application& dark(bool status);
			Application& fullscreen(bool status);
			Application& invoke(ArrayDynamicShort<String>&& cmd) noexcept
				{
				m_cmd_ready.wait();
				m_cmd_buffer[0]=std::move(cmd);
				m_ctx.messagePost(MessageId::INVOKE,0);
				return *this;
				}

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

			void muted(Engine& engine,int channel) noexcept;
			void unmuted(Engine& engine,int channel) noexcept;
			void recordDone(Engine& engine,int channel) noexcept;

			enum MessageId:int32_t
				{CHANNEL_MUTED,CHANNEL_UNMUTED,RECORD_DONE,INVOKE};
			typedef int32_t MessageParam;

			void process(UiContext& ctx,MessageId id,MessageParam param);

			template<class T>
			void layoutInspect(T&& cb) const
				{
				cb(0,m_session_control.boundingBox());
				cb(1,m_session_editor.boundingBoxKeyboard());
				cb(2,m_session_editor.boundingBoxTabs());
				}


		private:
			ImageRepository m_images;
			UiContext m_ctx;
			Session m_session;
			Window m_mainwin;
				Box m_cols;
					Anja::ButtonList m_session_control;
					Separator m_cols_sep;
					Box m_rows;
						Message m_status;
						Separator m_sep_a;
						Box m_ch_status;
							Filler m_ch_status_left;
							Label m_ch_status_label;
							ImageList m_ch_status_img;
							Filler m_ch_status_right;
						Separator m_sep_b;
						Anja::SessionEditor m_session_editor;
			bool m_fullscreen;

			std::unique_ptr<Dialog<Message,ConfirmSaveDialog> > m_confirm;
			void save_ask(ConfirmSaveDialogId id);

			std::unique_ptr<Dialog<AboutBox,AboutDialog>> m_about;
			std::unique_ptr<Dialog<Message,DialogOk> > m_error;

			std::unique_ptr<Engine> m_engine;
			std::bitset<256> m_keystate;
			int m_rec_count;

			ArrayDynamicShort<String> m_cmd_buffer[2];
			ReadySignal m_cmd_ready;

			void engine_stop();
			void engine_start();

			String filename_generate(int slot);
			void command_process(const ArrayDynamicShort<String>& cmd);
		};
	}

#endif
