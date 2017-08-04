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
#include "../common/targetinclude.hpp"
#include "statusicons.hpp"
#include <type_traits>
#include <inttypes.h>

using namespace Anja;

namespace{ struct ProgressAborted{}; }

ANJA_BLOB(uint8_t,s_logo,ANJA_TARGET(../logo_1.png));

static constexpr const char* ANJA_OFFLINE="Stopped";
static constexpr const char* ANJA_ONLINE="Running";
static constexpr const char* ANJA_RESTART_NEEDED="Restart needed";

static constexpr const char* ANJA_KEYB_ACTIVE="Keyboard";
static constexpr const char* ANJA_KEYB_INACTIVE="Keyboard";

static void title_update(const Session& session,Window& win)
	{
	String title(session.title());
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
	msg.append(m_session.title()).append("?");
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
			sessionLoad(m_session.filename().begin());
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
	{
	auto connected=[&engine](decltype(type) t,decltype(index) i)
		{
		switch(t)
			{
			case decltype(t)::WAVE_IN:
				return engine.waveInConnected(i);
			case decltype(t)::MIDI_IN:
				return engine.midiInConnected(i);
			case decltype(t)::WAVE_OUT:
				return engine.waveOutConnected(i);
			case decltype(t)::MIDI_OUT:
				return engine.midiOutConnected(i);
			}
		return false;
		}(type,index);
	if(!connected)
		{m_ctx.messagePost(MessageId::PORT_DISCONNECTED,(static_cast<uint32_t>(type)<<16)|index);}
	}

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

static void chlabels_update(Session& session,ImageList& chstatus)
	{
	for(int k=0;k<16;++k)
		{
		chstatus[k + 2].title(session.channelLabel(k).begin());
		}
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
					m_ch_status_img[param+2].showPng(m_images,static_cast<size_t>(StatusIcon::STOP)
						,statusIcon(StatusIcon::STOP));
					}
				else
					{
					m_ch_status_img[param+2].showPng(m_images,static_cast<size_t>(StatusIcon::OFF)
						,statusIcon(StatusIcon::OFF));
					}
				}
			else
				{
				m_ch_status_img[param+2].showPng(m_images,static_cast<size_t>(StatusIcon::STOP)
					,statusIcon(StatusIcon::STOP));
				}
			break;

		case MessageId::CHANNEL_UNMUTED:
			assert(param>=0 && param<16);
			if(m_engine->waveOutCount()>2 ) //Multi-channel mode
				{
				if(m_engine->waveOutConnected(param))
					{
					m_ch_status_img[param+2].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
						,statusIcon(StatusIcon::READY));
					}
				else
					{
					m_ch_status_img[param+2].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
						,statusIcon(StatusIcon::WAIT));
					}
				}
			else
				{
				m_ch_status_img[param+2].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
					,statusIcon(StatusIcon::READY));
				}
			break;

		case MessageId::PORT_CONNECTED:
			{
			auto port=param&0x00ff;
			auto type=static_cast<AudioClient::PortType>(param>>16);

			switch(type)
				{
				case AudioClient::PortType::WAVE_OUT:
					{
					if(m_engine->waveOutCount()>2)
						{
						if(port<16 && m_engine->muted(port))
							{
							m_ch_status_img[port+2].showPng(m_images,static_cast<size_t>(StatusIcon::STOP)
								,statusIcon(StatusIcon::STOP));
							}
						else
							{
							m_ch_status_img[port+2].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
								,statusIcon(StatusIcon::READY));
							}
						}
					else
						{
						m_ch_status_img[port + 16+2].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
							,statusIcon(StatusIcon::READY));
						}
					}
					break;

				case AudioClient::PortType::WAVE_IN:
					m_ch_status_img[0].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
						,statusIcon(StatusIcon::READY));
					break;

				case AudioClient::PortType::MIDI_IN:
					m_ch_status_img[1].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
						,statusIcon(StatusIcon::READY));
					break;

				case AudioClient::PortType::MIDI_OUT:
					m_ch_status_img[20].showPng(m_images,static_cast<size_t>(StatusIcon::READY)
						,statusIcon(StatusIcon::READY));
					break;
				}
			}
			break;

		case MessageId::PORT_DISCONNECTED:
			{
			auto port=param&0x00ff;
			auto type=static_cast<AudioClient::PortType>(param>>16);

			switch(type)
				{
				case AudioClient::PortType::WAVE_OUT:
					{
					if(m_engine->waveOutCount()>2)
						{
						if(port<16 && m_engine->muted(port))
							{
							m_ch_status_img[port + 2].showPng(m_images,static_cast<size_t>(StatusIcon::OFF)
								,statusIcon(StatusIcon::OFF));
							}
						else
							{
							m_ch_status_img[port + 2].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
								,statusIcon(StatusIcon::WAIT));
							}
						}
					else
						{
						m_ch_status_img[port + 16 + 2].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
							,statusIcon(StatusIcon::WAIT));
						}
					}
					break;

				case AudioClient::PortType::WAVE_IN:
					m_ch_status_img[0].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
						,statusIcon(StatusIcon::WAIT));
					break;

				case AudioClient::PortType::MIDI_IN:
					m_ch_status_img[1].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
						,statusIcon(StatusIcon::WAIT));
					break;

				case AudioClient::PortType::MIDI_OUT:
					m_ch_status_img[20].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
						,statusIcon(StatusIcon::WAIT));
					break;
				}
			}
			break;

		case MessageId::RECORD_DONE:
			{
			assert(param>=0 && param<128);
			auto scancode=slotToScancode(param);
			if(scancode==0xff)
				{return;}
			m_session.waveform(param).sampleRate(m_engine->sampleRate())
				.offsetsReset().recorded(true);
			if(m_keystate[scancode])
				{
				auto note=slotToMIDI(param);
				m_engine->messagePost(MIDIConstants::Message
					{
					 MIDIConstants::StatusCodes::NOTE_ON
					,static_cast<int>(m_session.waveform(param).channel())
					,note
					,127
					});
				}
			m_session.waveformProxy(param).filename(filename_generate(param));
			m_session.slotActive(param);
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

void Application::progressResample(WaveformProxy& waveform,float status)
	{
	if(m_progress_null)
		{
		String label("Resampling ");
		label.append(waveform.filename());
		m_progress_null->widget().value(status).label(label.begin());
		}
	}

void Application::engine_start()
	{
	std::for_each(m_ch_status_img.begin(),m_ch_status_img.end(),[this](ImageView& v)
		{
		v.showPng(m_images,static_cast<size_t>(StatusIcon::OFF),statusIcon(StatusIcon::OFF));
		});
	m_engine.reset( new Engine(m_session,*this) );
	m_ch_status_img[0].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
		,statusIcon(StatusIcon::WAIT));
	m_ch_status_img[1].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
		,statusIcon(StatusIcon::WAIT));
	m_ch_status_img[2 + 16 + 0].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
		,statusIcon(StatusIcon::WAIT));
	m_ch_status_img[2 + 16 + 1].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
		,statusIcon(StatusIcon::WAIT));
	m_ch_status_img[2 + 16 + 2].showPng(m_images,static_cast<size_t>(StatusIcon::WAIT)
		,statusIcon(StatusIcon::WAIT));

	String dlgcaption("Anja: Resampling waveforms");
	m_progress_null.reset(new Dialog<ProgressBox,DialogNull>(m_mainwin,dlgcaption.begin()));
	m_session.sampleRate(m_engine->sampleRate(),*this);
	m_progress_null.reset();
	m_session_editor.sessionUpdated();
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
	m_session.engineStopped();
	}



Application& Application::sessionNew()
	{
	auto running=(m_engine.get()!=nullptr);
	engine_stop();
	m_session.clear();
	m_session_editor.sessionUpdated();
	title_update(m_session,m_mainwin);
	chlabels_update(m_session,m_ch_status_img);
	if(running)
		{
		try
			{engine_start();}
		catch(...)
			{}
		}
	return *this;
	}

bool Application::sessionSave()
	{
	if(m_session.filename().length()!=0)
		{
		m_session.save(m_session.filename().begin());
		return 1;
		}
	return sessionSaveAs();
	}

bool Application::sessionSaveAs()
	{
	auto name=std::string(m_session.filename().begin());
	if(filenameSelect(m_mainwin,m_session.directory().begin()
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
	auto name=std::string(m_session.filename().begin());
	if(filenameSelect(m_mainwin,m_session.directory().begin()
		,name,Anja::FilenameSelectMode::OPEN,[this](const char* name)
			{return m_session.loadPossible(name);},"Anja session files"))
		{sessionLoad(name.c_str());}
	return *this;
	}



void Application::closing(Window& win,int id)
	{
	if(m_session.dirty())
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
				m_engine->messagePost(MIDIConstants::Message
					{
					 MIDIConstants::StatusCodes::NOTE_ON
					,static_cast<int>(m_session.waveform(slot).channel())
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
						auto slot_current=m_session.slotActive();
						assert(slot_current>=0 && slot_current<128);
						note=slotToMIDI(slot_current);
						m_engine->messagePost(MIDIConstants::Message{MIDIConstants::StatusCodes::NOTE_ON,0,note|0x80,127});
						}
						break;

					case Keys::KILL_ALL:
						for(size_t k=0;k<ChannelMixer::length();++k)
							{m_engine->messagePost(MIDIConstants::Message{MIDIConstants::ControlCodes::SOUND_OFF,static_cast<int>(k),0});}
						break;

					case Keys::KILL_AUDITION:
						m_engine->messagePost(MIDIConstants::Message{MIDIConstants::ControlCodes::SOUND_OFF,0,1});
						break;

					case 1:
						m_keyb_status_view.showPng(m_images
							,static_cast<size_t>(StatusIcon::READY),statusIcon(StatusIcon::READY));
						break;
					}
				}
			}
		}
	m_keystate[scancode]=1;
	}

void Application::focusIn(Window& win,int id)
	{
	m_keyb_status_view.showPng(m_images
		,static_cast<size_t>(StatusIcon::READY),statusIcon(StatusIcon::READY));
	}

void Application::focusOut(Window& win,int id)
	{
	m_keyb_status_view.showPng(m_images
		,static_cast<size_t>(StatusIcon::STOP),statusIcon(StatusIcon::STOP));
	}

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
				m_engine->messagePost(MIDIConstants::Message
					{
					 MIDIConstants::StatusCodes::NOTE_OFF
					,static_cast<int>(m_session.waveform(slot).channel())
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
					auto slot_current=m_session.slotActive();
					assert(slot_current>=0 && slot_current<128);
					note=slotToMIDI(slot_current);
					m_engine->messagePost(MIDIConstants::Message{MIDIConstants::StatusCodes::NOTE_OFF,0,note,127});
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
	if(cmd.length()==0)
		{return;}

	if(cmd[0].length()==0)
		{return;}

	fprintf(stderr,"Anja: %s",cmd[0].begin());
	std::for_each(cmd.begin()+1,cmd.end(),[](const String& str)
		{fprintf(stderr,",%s",str.begin());});
	fprintf(stderr,"\n");

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
	else
	if(cmd[0]=="port selector open")
		{
		auto port=cmd.length()>1?static_cast<size_t>( atoi(cmd[1].begin()) ):18;
		if(port>=0 && port<m_ch_status_img.size())
			{clicked(m_ch_status_img,0,m_ch_status_img[port]);}
		}
	else
	if(cmd[0]=="port selector close")
		{m_port_selector.reset();}
	else
	if(cmd[0]=="waveform load")
		{
		if(cmd.length()<3)
			{return;}
		auto slot=atoi(cmd[1].begin());
		if(slot>=0 && slot<128)
			{
			try
				{
				m_session.slotActive(slot);
				m_progress.reset(new Dialog<ProgressBox,DialogCancel>(m_mainwin,"Anja loading waveform"));
				m_progress->callback(*this,0);
				m_session.waveformProxy(slot).waveformLoad(cmd[2].begin(),*this);
				m_progress.reset();
				m_session_editor.sessionUpdated().waveformAutotrim();
				}
			catch(const ProgressAborted&)
				{}
			catch(...)
				{}
			}
		}
	else
	if(cmd[0]=="session load")
		{
		if(cmd.length()<2)
			{return;}
		sessionLoad(cmd[1].begin());
		}
	else
	if(cmd[0]=="settings")
		{
		if(cmd.length()<2)
			{return;}

		if(cmd[1]=="waveform")
			{m_session_editor.waveformShow();}
		else
		if(cmd[1]=="channels")
			{m_session_editor.mixerShow();}
		else
		if(cmd[1]=="session")
			{m_session_editor.sessionShow();}
		}
	}

static String port_title(const Session& session,int id)
	{
	assert(id>=2 && id<=19);
	switch(id)
		{
		case 18:
			return String("Master out: Port selection");
		case 19:
			return String("Audition: Port selection");
		default:
			return String(session.channelLabel(id-2)).append(": Port selection");
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

	switch(img.id())
		{
		case 0:
			m_port_selector.reset(new Dialog<PortSelector,DialogOkCancel>(m_mainwin
				,"Wave in: Port selection"));
			m_engine->waveOutEnum([this](AudioClient& client,const char* port_name)
				{
				m_port_selector->widget().portAppend(port_name);
				return true;
				});
			m_engine->waveInConnectionsEnum(0,[this](AudioClient& client,const char* port_name)
				{
				m_port_selector->widget().select(port_name);
				return true;
				});
			break;

		case 1:
			m_port_selector.reset(new Dialog<PortSelector,DialogOkCancel>(m_mainwin
				,"MIDI in: Port selection"));
			m_engine->midiOutEnum([this](AudioClient& client,const char* port_name)
				{
				m_port_selector->widget().portAppend(port_name);
				return true;
				});
			m_engine->midiInConnectionsEnum(0,[this](AudioClient& client,const char* port_name)
				{
				m_port_selector->widget().select(port_name);
				return true;
				});
			break;

		case 20:
			m_port_selector.reset(new Dialog<PortSelector,DialogOkCancel>(m_mainwin
				,"MIDI out: Port selection"));
			m_engine->midiInEnum([this](AudioClient& client,const char* port_name)
				{
				m_port_selector->widget().portAppend(port_name);
				return true;
				});
			m_engine->midiOutConnectionsEnum(0,[this](AudioClient& client,const char* port_name)
				{
				m_port_selector->widget().select(port_name);
				return true;
				});
			break;

		default:
			{
			if(m_engine->waveOutCount()>2 || (m_engine->waveOutCount()<=2 && img.id()>=16 + 2))
				{
				auto title=port_title(m_session,img.id());
				auto port_index=m_engine->waveOutCount()>2?img.id()-2:img.id() - 16 - 2;

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
		}


	m_port_selector->callback(*this,img.id());
	m_port_selector->show();
	}

void Application::dismiss(Dialog<PortSelector,DialogOkCancel>& dlg,int id)
	{
	m_port_selector.reset(nullptr);
	}

void Application::confirmPositive(Dialog<PortSelector,DialogOkCancel>& dlg,int id)
	{
	dlg.widget().state([this,id](const char* port,bool status)
		{
		switch(id)
			{
			case 0:
				if(status)
					{m_engine->waveInConnect(0,port);}
				else
					{m_engine->waveInDisconnect(0,port);}
				break;
			case 1:
				if(status)
					{m_engine->midiInConnect(0,port);}
				else
					{m_engine->midiInDisconnect(0,port);}
				break;

			case 20:
				if(status)
					{m_engine->midiOutConnect(0,port);}
				else
					{m_engine->midiOutDisconnect(0,port);}
				break;

			default:
				{
				auto port_index=m_engine->waveOutCount()>2?id-2:id - 16 - 2;
				if(status)
					{m_engine->waveOutConnect(port_index,port);}
				else
					{m_engine->waveOutDisconnect(port_index,port);}
				}
				break;
			}
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
				if(m_session.dirty())
					{save_ask(ConfirmSaveDialogId::SESSION_NEW);}
				else
					{sessionNew();}
				break;
			case 1:
				if(m_session.dirty())
					{save_ask(ConfirmSaveDialogId::SESSION_LOAD);}
				else
					{sessionLoad();}
				break;
			case 2:
				if(m_session.dirty())
					{save_ask(ConfirmSaveDialogId::SESSION_RELOAD);}
				else
					{sessionLoad(m_session.filename().begin());}
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
				if(m_session.dirty())
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
		m_error.reset(new Dialog<Message,DialogOk>(m_mainwin,"Anja: Error",m_images
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
	if(id==0 && (1<<option)==Session::MULTIOUTPUT && m_engine)
		{m_status.message(ANJA_RESTART_NEEDED).type(Message::Type::WAIT);}
	}

void Application::progressLoad(WaveformProxy& waveform,float status)
	{
	if(!m_progress)
		{throw ProgressAborted{};}
	String label("Loading ");
	label.append(waveform.filename());
	m_progress->widget().value(status).label(label.begin());

	}

void Application::dismiss(Dialog<ProgressBox,DialogCancel>& dlg,int id)
	{
	m_progress.reset();
	}


Application& Application::sessionLoad(const char* filename)
	{
	auto engine_running=( m_engine.get()!=nullptr );
	engine_stop();
	try
		{
		String dlgcaption("Anja: Loading session ");
		dlgcaption.append(filename);
		m_progress.reset(new Dialog<ProgressBox,DialogCancel>(m_mainwin,dlgcaption.begin()));
		m_progress->callback(*this,0);
		m_session.load(filename,*this);
		m_progress.reset();
		m_session_editor.sessionUpdated();
		title_update(m_session,m_mainwin);
		chlabels_update(m_session,m_ch_status_img);
		}
	catch(const ProgressAborted&)
		{
	//	Swallow...
		}
	catch(...)
		{
		m_progress.reset();
		if(engine_running)
			{
			try
				{engine_start();}
			catch(...)
				{}
			}
		throw;
		}
	if(engine_running)
		{
		try
			{engine_start();}
		catch(...)
			{}
		}
	return *this;
	}

void Application::nameChanged(ChannelStrip& strip,int id)
	{
	if(m_engine && m_session.flags()&Session::MULTIOUTPUT)
		{m_engine->waveOutName(id,strip.name().begin());}
	m_ch_status_img[id + 2].title(strip.name().begin());
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
				,m_sep_a(m_status_row.insertMode({2,Box::EXPAND|Box::FILL}),true)
				,m_ch_status(m_status_row.insertMode({0,0}),false)
					,m_ch_status_label(m_ch_status.insertMode({2,0}),"Ports:")
					,m_ch_status_img(m_ch_status.insertMode({0,0}),false)
				,m_sep_b(m_status_row.insertMode({4,Box::EXPAND|Box::FILL}),true)
				,m_keyb_status(m_status_row.insertMode({0,0}),false)
					,m_keyb_status_label(m_keyb_status.insertMode({0,0}),"Keyboard:")
					,m_keyb_status_view(m_keyb_status.insertMode({2,0}))
				,m_sep_c(m_status_row.insertMode({2,0}),true)
				,m_mem(m_status_row.insertMode({0,0}),false)
					,m_mem_label(m_mem.insertMode({2,0}),"Mem:")
					,m_mem_view(m_mem.insertMode({0,0}))
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
	m_ch_status_img.append<2>()
		.separator()
		.append<ChannelMixer::length()>()
		.separator()
		.append<3>();
	std::for_each(m_ch_status_img.begin(),m_ch_status_img.end(),[this](ImageView& v)
		{
		v.minHeight(22)
			.showPng(m_images,static_cast<size_t>(StatusIcon::OFF),statusIcon(StatusIcon::OFF))
			.padding(4);
		});
	m_ch_status_img[0].backgroundShade(0.66,0.7f).title("Wave in");
	m_ch_status_img[1].backgroundShade(0.0,0.7f).title("MIDI in");
	m_ch_status_img[18].backgroundShade(0.66,0.7f).title("Master out");
	m_ch_status_img[19].backgroundShade(0.66,0.7f).title("Audition");
	m_ch_status_img[20].backgroundShade(0.0,0.7f).title("MIDI out");
	m_ch_status_img.callback(*this,0);
	chlabels_update(m_session,m_ch_status_img);
	m_keyb_status_view.minHeight(18);

	m_mainwin.icon(m_images,StatusIconEnd,{s_logo_begin,s_logo_end}).show();
	}
