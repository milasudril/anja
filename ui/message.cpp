//@	{
//@	 "targets":
//@		[{
//@		"name":"message.o","type":"object","cxxoptions":{"cflags_extra":["fno-lto"]}
//@		,"dependencies":
//@			[
//@				 {"ref":"message_error.png","rel":"generated"}
//@				,{"ref":"message_info.png","rel":"generated"}
//@				,{"ref":"message_warning.png","rel":"generated"}
//@				,{"ref":"led_stop.png","rel":"generated"}
//@				,{"ref":"led_wait.png","rel":"generated"}
//@				,{"ref":"led_ready.png","rel":"generated"}
//@			]
//@		}]
//@	}


#include "message.hpp"
#include "../common/blob.hpp"
#include <maike/targetinclude.hpp>

ANJA_BLOB(uint8_t,message_error,MAIKE_TARGET(message_error.png));
ANJA_BLOB(uint8_t,message_info,MAIKE_TARGET(message_info.png));
ANJA_BLOB(uint8_t,message_warning,MAIKE_TARGET(message_warning.png));

ANJA_BLOB(uint8_t,led_stop,MAIKE_TARGET(led_stop.png));
ANJA_BLOB(uint8_t,led_wait,MAIKE_TARGET(led_wait.png));
ANJA_BLOB(uint8_t,led_ready,MAIKE_TARGET(led_ready.png));

using namespace Anja;

Message::Message(Container& cnt,const char* message,Type t,bool wordwrap):
	m_cols(cnt,false)
		,m_icon(m_cols.insertMode({2,0}))
		,m_text(m_cols.insertMode(
			{2,static_cast<unsigned short>(wordwrap?(Box::EXPAND|Box::FILL):0)})
			,message)
	{
	m_text.wordwrap(wordwrap);
	if(!wordwrap)
		{m_icon.minHeight(18);}
	type(t);
	}

Message& Message::type(Type type)
	{
	switch(type)
		{
		case Type::ERROR:
			m_icon.showPng(message_error_begin,message_error_end);
			break;
		case Type::INFORMATION:
			m_icon.showPng(message_info_begin,message_info_end);
			break;
		case Type::WARNING:
			m_icon.showPng(message_warning_begin,message_warning_end);
			break;
		case Type::STOP:
			m_icon.showPng(led_stop_begin,led_stop_end);
			break;
		case Type::WAIT:
			m_icon.showPng(led_wait_begin,led_wait_end);
			break;
		case Type::READY:
			m_icon.showPng(led_ready_begin,led_ready_end);
			break;

		}
	return *this;
	}

Message& Message::message(const char* msg)
	{
	m_text.content(msg);
	return *this;
	}
