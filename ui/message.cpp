//@	{
//@	 "targets":
//@		[{
//@		"name":"message.o","type":"object","cxxoptions":{"cflags_extra":["fno-lto"]}
//@		,"dependencies":
//@			[
//@				 {"ref":"message_error.png","rel":"generated"}
//@				,{"ref":"message_info.png","rel":"generated"}
//@				,{"ref":"message_warning.png","rel":"generated"}
//@			]
//@		}]
//@	}


#include "message.hpp"
#include "../common/blob.hpp"
#include <maike/targetinclude.hpp>

ANJA_BLOB(uint8_t,message_error,MAIKE_TARGET(message_error.png));
ANJA_BLOB(uint8_t,message_info,MAIKE_TARGET(message_info.png));
ANJA_BLOB(uint8_t,message_warning,MAIKE_TARGET(message_warning.png));

using namespace Anja;

Message::Message(Container& cnt,const char* message,Type type):
	m_cols(cnt,false)
		,m_icon(m_cols.insertMode({4,0}))
		,m_text(m_cols.insertMode({0,Box::EXPAND|Box::FILL}),message)
	{
	m_text.wrap(1);
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
		}
	}
