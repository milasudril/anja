//@	{
//@	"targets":
//@		[{
//@		"name":"application.o","type":"object"
//@		,"dependencies":[{"ref":"../logo_1.png","rel":"misc"}]
//@		,"cxxoptions":{"cflags_extra":["fno-lto"]}
//@		,"include_targets":["../projectinfo.hpp"]
//@		}]
//@	}

#include "application.hpp"
#include "filenameselect.hpp"
#include "../common/blob.hpp"
#include "../sessiondata/keymap.hpp"
#include "statusicons.hpp"
#include <inttypes.h>
#include <maike/targetinclude.hpp>

using namespace Anja;

ANJA_BLOB(uint8_t,s_logo,MAIKE_TARGET(../logo_1.png));

static constexpr const char* ANJA_OFFLINE="Stopped";
static constexpr const char* ANJA_ONLINE="Running";
static constexpr const char* ANJA_RESTART_NEEDED="Restart needed";

static constexpr const char* ANJA_KEYB_ACTIVE="Keyboard active";
static constexpr const char* ANJA_KEYB_INACTIVE="Keyboard inactive";

static void title_update(const Session& session,Window& win)
	{
	String title(session.titleGet());
	title.append("—Anja");
	win.title(title.begin());
	}

void Application::save_ask(ConfirmSaveDialogId id)
	{
	const char* title="";
	switch(id)
		{
		case ConfirmSaveDialogId::EXIT:
			title="Anja: Closing Anja";
			break;
		case ConfirmSaveDialogId::SESSION_LOAD:
			title="Anja: Loading another session";
			break;
		case ConfirmSaveDialogId::SESSION_RELOAD:
			title="Anja: Reloading session";
			break;
		case ConfirmSaveDialogId::SESSION_NEW:
			title="Anja: Creating a new session";
			break;
		}
	String msg("Do you want to save changes to ");
	msg.append(m_session.titleGet()).append("?");
	m_confirm.reset(new Dialog<Message,ConfirmSaveDialog>(m_mainwin,title,m_images
		,msg.begin(),Message::Type::WARNING));
	m_confirm->callback(*this,id);
	}

void Application::dismiss(Dialog<Message,ConfirmSaveDialog>& dlg,ConfirmSaveDialogId id)
	{
	m_confirm.reset();
	}

void Application::confirmPositive(Dialog<Message,ConfirmSaveDialog>& dlg,ConfirmSaveDialogId id)
	{
	if(sessionSave())
		{confirmNegative(dlg,id);}
	}

void Application::confirmNegative(Dialog<Message,ConfirmSaveDialog>& dlg,ConfirmSaveDialogId id)
	{
	m_confirm.reset();
	switch(id)
		{
		case ConfirmSaveDialogId::EXIT:
			m_ctx.exit();
			break;
		case ConfirmSaveDialogId::SESSION_LOAD:
			sessionLoad();
			break;
		case ConfirmSaveDialogId::SESSION_RELOAD:
			sessionLoad(m_session.filenameGet().begin());
			break;
		case ConfirmSaveDialogId::SESSION_NEW:
			sessionNew();
			break;
		}
	}

void Application::muted(Engine& engine,int channel) noexcept
	{
	m_ctx.messagePostTry(MessageId::CHANNEL_MUTED,channel);
	}

void Application::unmuted(Engine& engine,int channel) noexcept
	{
	m_ctx.messagePostTry(MessageId::CHANNEL_UNMUTED,channel);
	}

void Application::recordDone(Engine& engine,int slot) noexcept
	{
	m_ctx.messagePostTry(MessageId::RECORD_DONE,slot);
	}

void Application::portConnected(Engine& engine,AudioClient::PortType type,int index) noexcept
	{m_ctx.messagePost(MessageId::PORT_CONNECTED,(static_cast<uint32_t>(type)<<16)|index);}

void Application::portDisconnected(Engine& engine,AudioClient::PortType type,int index) noexcept
	{m_ctx.messagePost(MessageId::PORT_DISCONNECTED,(static_cast<uint32_t>(type)<<16)|index);}

String Application::filename_generate(int slot)
	{
	String ret(m_session.waveformData(slot).keyLabel());
	if(ret.length()!=0)
		{ret.append('-');}
	char buff[64];
	sprintf(buff,"%" PRIx64 "-%x",wallclock(),m_rec_count);
	++m_rec_count;
	ret.append(buff).append(".wav");
	return ret;
	}

void Application::process(UiContext& ctx,MessageId id,MessageParam param)
	{
	switch(id)
		{
		case MessageId::CHANNEL_MUTED:
			assert(param>=0 && param<16);
			if(m_engine->waveOutCount()>2 ) //Multi-channel mode
				{
				if(m_engine->waveOutConnected(param))
					{
					m_ch_status_img[param].showPng(m_images,static_cast<size_t>(StatusIcon::STOP)
						,statusIcon(StatusIcon::STOP));
					}
				else
					{
					m_ch_status_img[param].showPng(m_images,static_cast<size_t>(StatusIcon::OFF)
						,statusIcon(StatusIcon::OFF));
					}
				}
			else
				{
				m_ch_status_img[param].showPng(m_images,static_cast<size_t>(StatusIcon::STOP)
					,statusIcon(StatusIcon::STOP));
				}
			break;

		case MessageId::CHANNEL_UNMUTED:
			assert(param>=0 && param<16);
			if(m_engine->waveOutCount()>2 ) //Multi-channel mode
				{
				if(m_engine->waveOutConnected(param))
					{
					m_ch_status_img[param].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
						,statusIcon(StatusIcon::READY));
					}
				else
					{
					m_ch_status_img[param].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
						,statusIcon(StatusIcon::WAIT));
					}
				}
			else
				{
				m_ch_status_img[param].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
					,statusIcon(StatusIcon::READY));
				}
			break;

		case MessageId::PORT_CONNECTED:
			{
			auto port=param&0x00ff;
			auto type=static_cast<AudioClient::PortType>(param>>16);

			if(type==AudioClient::PortType::WAVE_OUT)
				{
				if(m_engine->waveOutCount()>2)
					{
					if(port<16 && m_engine->muted(port))
						{
						m_ch_status_img[port].showPng(m_images,static_cast<size_t>(StatusIcon::STOP)
							,statusIcon(StatusIcon::STOP));
						}
					else
						{
						m_ch_status_img[port].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
							,statusIcon(StatusIcon::READY));
						}
					}
				else
					{
					m_ch_status_img[port + 16].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
						,statusIcon(StatusIcon::READY));
					}
				}
			}
			break;

		case MessageId::PORT_DISCONNECTED:
			{
			auto port=param&0x00ff;
			auto type=static_cast<AudioClient::PortType>(param>>16);

			if(type==AudioClient::PortType::WAVE_OUT)
				{
				if(m_engine->waveOutCount()>2)
					{
					if(port<16 && m_engine->muted(port))
						{
						m_ch_status_img[port].showPng(m_images,static_cast<size_t>(StatusIcon::OFF)
							,statusIcon(StatusIcon::OFF));
						}
					else
						{
						m_ch_status_img[port].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
							,statusIcon(StatusIcon::WAIT));
						}
					}
				else
					{
					m_ch_status_img[port + 16].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
						,statusIcon(StatusIcon::WAIT));
					}
				}
			}
			break;

		case MessageId::RECORD_DONE:
			{
			assert(param>=0 && param<128);
			auto scancode=slotToScancode(param);
			if(scancode==0xff)
				{return;}
			m_session.waveformGet(param).sampleRate(m_engine->sampleRate())
				.offsetsReset().flagsSet(Waveform::RECORDED);
			if(m_keystate[scancode])
				{
				auto note=slotToMIDI(param);
				m_engine->messagePost(MIDI::Message
					{
					 MIDI::StatusCodes::NOTE_ON
					,static_cast<int>(m_session.waveformGet(param).channel())
					,note
					,127
					});
				}
			m_session.waveformViewGet(param).filename(filename_generate(param));
			m_session.slotActiveSet(param);
			m_session_editor.sessionUpdated();
			}
			break;

		case MessageId::INVOKE:
			std::swap(m_cmd_buffer[0],m_cmd_buffer[1]);
			m_cmd_ready.set();
			command_process(m_cmd_buffer[1]);
			break;
		}
	}

void Application::engine_start()
	{
	std::for_each(m_ch_status_img.begin(),m_ch_status_img.end(),[this](ImageView& v)
		{
		v.showPng(m_images,static_cast<size_t>(StatusIcon::OFF),statusIcon(StatusIcon::OFF));
		});
	m_engine.reset( new Engine(m_session,*this) );
	m_ch_status_img[16].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT),statusIcon(StatusIcon::WAIT));
	m_ch_status_img[17].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT),statusIcon(StatusIcon::WAIT));

	m_status.message(ANJA_ONLINE).type(Message::Type::READY);
	}

void Application::engine_stop()
	{
	m_engine.reset();
	m_status.message(ANJA_OFFLINE).type(Message::Type::STOP);
	std::for_each(m_ch_status_img.begin(),m_ch_status_img.end(),[this](ImageView& v)
		{
		v.showPng(m_images,static_cast<size_t>(StatusIcon::OFF),statusIcon(StatusIcon::OFF));
		});

	}

Application& Application::sessionNew()
	{
	engine_stop();
	m_session.clear();
	m_session_editor.sessionUpdated();
	title_update(m_session,m_mainwin);
	try
		{engine_start();}
	catch(...)
		{}
	return *this;
	}

bool Application::sessionSave()
	{
	if(m_session.filenameGet().length()!=0)
		{
		m_session.save(m_session.filenameGet().begin());
		return 1;
		}
	return sessionSaveAs();
	}

bool Application::sessionSaveAs()
	{
	auto name=std::string(m_session.filenameGet().begin());
	if(filenameSelect(m_mainwin,m_session.directoryGet().begin()
		,name,Anja::FilenameSelectMode::SAVE))
		{
		m_session.save(name.c_str());
		m_session_editor.sessionUpdated();
		return 1;
		}
	return 0;
	}

Application& Application::sessionLoad()
	{
	auto name=std::string(m_session.filenameGet().begin());
	if(filenameSelect(m_mainwin,m_session.directoryGet().begin()
		,name,Anja::FilenameSelectMode::OPEN,[this](const char* name)
			{return m_session.loadPossible(name);},"Anja session files"))
		{sessionLoad(name.c_str());}
	return *this;
	}



void Application::closing(Window& win,int id)
	{
	if(m_session.dirtyIs())
		{save_ask(ConfirmSaveDialogId::EXIT);}
	else
		{m_ctx.exit();}
	}

UiContext::RunStatus Application::idle(UiContext& ctx)
	{
	return UiContext::RunStatus::WAIT;
	}

void Application::keyDown(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id)
	{
	if(m_engine && !m_keystate[scancode])
		{
		auto note=scancodeToMIDI(scancode);
		if(note!=0xff)
			{
			auto slot=scancodeToSlot(scancode);
			if(keymask&KEYMASK_KEY_CTRL)
				{m_engine->recordStop(note).recordStart(note);}
			else
				{
				m_engine->messagePost(MIDI::Message
					{
					 MIDI::StatusCodes::NOTE_ON
					,static_cast<int>(m_session.waveformGet(slot).channel())
					,note
					,127
					});
				}
			}
		else
			{
			auto ch=scancodeToChannel(scancode);
			if(ch>=0 && ch<16)
				{
				if(m_keystate[Keys::FADE_IN])
					{m_engine->fadeIn(ch,m_session.channel(ch).fadeTime());}
				else
				if(m_keystate[Keys::FADE_OUT])
					{m_engine->fadeOut(ch,m_session.channel(ch).fadeTime());}
				else
				if(m_keystate[Keys::FADE_IN_FAST])
					{m_engine->fadeIn(ch,1e-3f);}
				else
				if(m_keystate[Keys::FADE_OUT_FAST])
					{m_engine->fadeOut(ch,1e-3f);}
				}
			else
				{
				switch(scancode)
					{
					case Keys::AUDITION:
						{
						auto slot_current=m_session.slotActiveGet();
						assert(slot_current>=0 && slot_current<128);
						note=slotToMIDI(slot_current);
						m_engine->messagePost(MIDI::Message{MIDI::StatusCodes::NOTE_ON,0,note|0x80,127});
						}
						break;

					case Keys::KILL_ALL:
						for(size_t k=0;k<ChannelMixer::length();++k)
							{m_engine->messagePost(MIDI::Message{MIDI::ControlCodes::SOUND_OFF,static_cast<int>(k),0});}
						break;

					case 1:
						m_keyb_status.message(ANJA_KEYB_ACTIVE).type(Message::Type::READY);
						break;
					}
				}
			}
		}
	m_keystate[scancode]=1;
	}

void Application::focusIn(Window& win,int id)
	{m_keyb_status.message(ANJA_KEYB_ACTIVE).type(Message::Type::READY);}

void Application::focusOut(Window& win,int id)
	{m_keyb_status.message(ANJA_KEYB_INACTIVE).type(Message::Type::STOP);}

void Application::keyUp(Anja::Window& win,int scancode,Anja::keymask_t keymask,int id)
	{
	if(m_engine)
		{
		auto note=scancodeToMIDI(scancode);
		if(note!=0xff)
			{
			auto slot=scancodeToSlot(scancode);
			if(keymask&KEYMASK_KEY_CTRL || scancode==Keys::RECORD_START_L
				|| scancode==Keys::RECORD_START_R)
				{m_engine->recordStop(note);}
			else
				{
				m_engine->messagePost(MIDI::Message
					{
					 MIDI::StatusCodes::NOTE_OFF
					,static_cast<int>(m_session.waveformGet(slot).channel())
					,note
					,127
					});
				}
			}
		else
			{
			switch(scancode)
				{
				case Keys::AUDITION:
					{
					auto slot_current=m_session.slotActiveGet();
					assert(slot_current>=0 && slot_current<128);
					note=slotToMIDI(slot_current);
					m_engine->messagePost(MIDI::Message{MIDI::StatusCodes::NOTE_OFF,0,note,127});
					}
					break;

				case Keys::RECORD_START_L:
				case Keys::RECORD_START_R:
					//TODO: Special message to stop all recording processes?
					m_engine->recordStop(0);
					break;
				}

			}
		}
	m_keystate[scancode]=0;
	}


Application& Application::dark(bool status)
	{
	m_ctx.dark(status);
	m_session_control[8].label(m_ctx.dark()?"Light UI":"Dark UI");
	return *this;
	}

Application& Application::fullscreen(bool status)
	{
	m_fullscreen=status;
	m_mainwin.fullscreen(m_fullscreen);
	m_session_control[7].label(m_fullscreen?"Windowed":"Fullscreen");
	return *this;
	}

void Application::command_process(const ArrayDynamicShort<String>& cmd)
	{
	if(cmd[0]=="exit")
		{m_ctx.exit();}
	else
	if(cmd[0]=="layout inspect")
		{
		layoutInspect([](int k,const Rectangle& box)
			{
			printf("%d %.15g %.15g %.15g %.15g\n",k,box.min.x(),box.min.y()
				,box.max.x(),box.max.y());
			});
		}
	}

static String port_title(const Session& session,int id)
	{
	switch(id)
		{
		case 16:
			return String("Master out: Port selection");
		case 17:
			return String("Audition: Port selection");
		default:
			return String(session.channelLabelGet(id)).append(": Port selection");
		}
	}

void Application::clicked(ImageList& imglist,int id,ImageView& img)
	{
	if(!m_engine)
		{
		try
			{engine_start();}
		catch(const Error& e)
			{
			m_error.reset(new Dialog<Message,DialogOk>(m_mainwin,"Anja error",m_images
				,e.message(),Message::Type::ERROR));
			m_error->callback(*this,0);
			return;
			}
		}

	if(m_engine->waveOutCount()>2 || (m_engine->waveOutCount()<=2 && img.id()>=16))
		{
		auto title=port_title(m_session,img.id());
		auto port_index=m_engine->waveOutCount()>2?img.id():img.id() - 16;

		m_port_selector.reset(new Dialog<PortSelector,DialogOkCancel>(m_mainwin,title.begin()));

		m_engine->waveInEnum([this](AudioClient& client,const char* port_name)
			{
			m_port_selector->widget().portAppend(port_name);
			return true;
			});

		m_engine->waveOutConnectionsEnum(port_index,[this](AudioClient& client,const char* port_name)
			{
			m_port_selector->widget().select(port_name);
			return true;
			});

		m_port_selector->callback(*this,port_index);
		m_port_selector->show();
		}
	else
		{
		m_error.reset(new Dialog<Message,DialogOk>(m_mainwin,"Anja port selection"
			,m_images,"The output from this bus is hardwired in single channel "
					"mode. To activate multichannel mode, use the option "
					"\"Use individual ports for each channel\" found in the "
					"\"Session\" panel.",Message::Type::USER_ERROR));
		m_error->callback(*this,0);
		return;
		}
	}

void Application::dismiss(Dialog<PortSelector,DialogOkCancel>& dlg,int id)
	{
	m_port_selector.reset(nullptr);
	}

void Application::confirmPositive(Dialog<PortSelector,DialogOkCancel>& dlg,int id)
	{
	dlg.widget().state([this,id](const char* port,bool status)
		{
		if(status)
			{m_engine->waveOutConnect(id,port);}
		else
			{m_engine->waveOutDisconnect(id,port);}
		});

	m_port_selector.reset(nullptr);
	}


void Application::clicked(ButtonList& buttons,int id,Button& btn)
	{
	try
		{
		switch(btn.id())
			{
			case 0:
				if(m_session.dirtyIs())
					{save_ask(ConfirmSaveDialogId::SESSION_NEW);}
				else
					{sessionNew();}
				break;
			case 1:
				if(m_session.dirtyIs())
					{save_ask(ConfirmSaveDialogId::SESSION_LOAD);}				else
					{sessionLoad();}
				break;
			case 2:
				if(m_session.dirtyIs())
					{save_ask(ConfirmSaveDialogId::SESSION_RELOAD);}
				else
					{sessionLoad(m_session.filenameGet().begin());}
				break;
			case 3:
				sessionSave();
				break;
			case 4:
				sessionSaveAs();
				break;
			case 5:
				engine_start();
				break;
			case 6:
				engine_stop();
				break;
			case 7:
				m_fullscreen=!m_fullscreen;
				m_mainwin.fullscreen(m_fullscreen);
				btn.label(m_fullscreen?"Windowed":"Fullscreen");
				break;
			case 8:
				m_ctx.dark(!m_ctx.dark());
				btn.label(m_ctx.dark()?"Light UI":"Dark UI");
				break;

			case 9:
				if(m_session.dirtyIs())
					{save_ask(ConfirmSaveDialogId::EXIT);}
				else
					{m_ctx.exit();}
				break;

			case 10:
				m_about.reset(new Dialog<AboutBox,AboutDialog>(m_mainwin,"About Anja",ProjectInfo{}));
				m_about->widget().logo(m_images,StatusIconEnd,{s_logo_begin,s_logo_end},192);
				m_about->callback(*this,0);
				break;
			}
		}
	catch(const Error& e)
		{
		m_error.reset(new Dialog<Message,DialogOk>(m_mainwin,"Anja error",m_images
			,e.message(),Message::Type::ERROR));
		m_error->callback(*this,0);
		}
	btn.state(0);
	}

void Application::confirmPositive(Dialog<Message,DialogOk>& dlg,int id)
	{m_error.reset();}

void Application::confirmPositive(Dialog<AboutBox,AboutDialog>& dlg,int id)
	{m_about.reset();}

void Application::user1(Dialog<AboutBox,AboutDialog>& dlg,int id)
	{dlg.widget().legalBrief();}

void Application::user2(Dialog<AboutBox,AboutDialog>& dlg,int id)
	{dlg.widget().techstring();}

void Application::titleChanged(SessionPropertiesEditor& editor,int id)
	{
	title_update(m_session,m_mainwin);
	}

void Application::descriptionChanged(SessionPropertiesEditor& editor,int id)
	{}

void Application::optionChanged(SessionPropertiesEditor& editor,int id,int option)
	{
	if(id==0 && (1<<option)==Session::MULTIOUTPUT)
		{m_status.message(ANJA_RESTART_NEEDED).type(Message::Type::WAIT);}
	}


Application& Application::sessionLoad(const char* filename)
	{
	engine_stop();
	try
		{
		m_session.load(filename);
		m_session_editor.sessionUpdated();
		title_update(m_session,m_mainwin);
		}
	catch(...)
		{
		try
			{engine_start();}
		catch(...)
			{}
		throw;
		}
	try
		{engine_start();}
	catch(...)
		{}
	return *this;
	}

void Application::nameChanged(ChannelStrip& strip,int id)
	{
	if(m_engine && m_session.flagsGet()&Session::MULTIOUTPUT)
		{m_engine->waveOutName(id,strip.name().begin());}
	}

void Application::gainChanged(ChannelStrip& strip,int id)
	{
	if(m_engine)
		{m_engine->channelGain(id,strip.gain());}
	}

Application::Application():
	m_mainwin("New session--Anja")
		,m_rows(m_mainwin,true)
			,m_status_row(m_rows,false)
				,m_status(m_status_row,m_images,ANJA_OFFLINE,Message::Type::STOP,0)
				,m_sep_a(m_status_row.insertMode({0,Box::EXPAND|Box::FILL}),true)
				,m_ch_status(m_status_row.insertMode({0,0}),false)
					,m_ch_status_label(m_ch_status.insertMode({0,0}),"Bus status:")
					,m_ch_status_img(m_ch_status.insertMode({0,0}),false)
				,m_sep_b(m_status_row.insertMode({0,Box::EXPAND|Box::FILL}),true)
				,m_keyb_status(m_status_row.insertMode({0,0}),m_images,ANJA_KEYB_INACTIVE,Message::Type::STOP,0)
			,m_row_sep(m_rows,false)
			,m_cols(m_rows.insertMode({0,Box::EXPAND|Box::FILL}),false)
				,m_session_control(m_cols,true)
				,m_cols_sep(m_cols.insertMode({2,0}),true)
				,m_session_editor(m_cols.insertMode({2,Box::EXPAND|Box::FILL}),m_images,m_session)
	,m_fullscreen(0)
	,m_rec_count(0)
	{
	m_session_control.append("New session","Load session","Reload session","Save session"
		,"Save session as","","Start engine","Stop engine","","Fullscreen","Dark UI",""
		,"Exit","About Anja");
	m_session_control.callback(*this,0);
	m_mainwin.callback(*this,0);
	m_session_editor.callback(*this,0);
	title_update(m_session,m_mainwin);
	m_ch_status_img.append<ChannelMixer::length()>();
	m_ch_status_img.separator();
	m_ch_status_img.append<2>();
	std::for_each(m_ch_status_img.begin(),m_ch_status_img.end(),[this](ImageView& v)
		{
		v.minHeight(20)
			.showPng(m_images,static_cast<size_t>(StatusIcon::OFF),statusIcon(StatusIcon::OFF));
		});
	m_ch_status_img.callback(*this,0);

	m_mainwin.icon(m_images,StatusIconEnd,{s_logo_begin,s_logo_end}).show();
	try
		{engine_start();}
	catch(...)
		{}
	}
