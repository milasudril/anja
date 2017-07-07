//@	{
//@	 "targets":
//@		[{
//@		"name":"statusicons.o","type":"object"
//@		,"dependencies":
//@			[
//@				 {"ref":"../icons/message_error.png","rel":"misc"}
//@				,{"ref":"../icons/message_info.png","rel":"misc"}
//@				,{"ref":"../icons/message_warning.png","rel":"misc"}
//@				,{"ref":"../icons/message_usererror.png","rel":"misc"}
//@				,{"ref":"../icons/led_stop.png","rel":"misc"}
//@				,{"ref":"../icons/led_wait.png","rel":"misc"}
//@				,{"ref":"../icons/led_ready.png","rel":"misc"}
//@				,{"ref":"../icons/led_off.png","rel":"misc"}
//@			]
//@		,"cxxoptions":{"cflags_extra":["fno-lto"]}
//@		}]
//@	}

#include "statusicons.hpp"

using namespace Anja;

#include "../common/blob.hpp"
#include "../common/targetinclude.hpp"

ANJA_BLOB(uint8_t,message_error,ANJA_TARGET(../icons/message_error.png));
ANJA_BLOB(uint8_t,message_usererror,ANJA_TARGET(../icons/message_usererror.png));
ANJA_BLOB(uint8_t,message_info,ANJA_TARGET(../icons/message_info.png));
ANJA_BLOB(uint8_t,message_warning,ANJA_TARGET(../icons/message_warning.png));

ANJA_BLOB(uint8_t,led_stop,ANJA_TARGET(../icons/led_stop.png));
ANJA_BLOB(uint8_t,led_wait,ANJA_TARGET(../icons/led_wait.png));
ANJA_BLOB(uint8_t,led_ready,ANJA_TARGET(../icons/led_ready.png));
ANJA_BLOB(uint8_t,led_off,ANJA_TARGET(../icons/led_off.png));

std::pair<const uint8_t*,const uint8_t*> Anja::statusIcon(StatusIcon id) noexcept
	{
	switch(id)
		{
		case StatusIcon::ERROR:
			return {message_error_begin,message_error_end};

		case StatusIcon::USER_ERROR:
			return {message_usererror_begin,message_usererror_end};

		case StatusIcon::INFORMATION:
			return {message_info_begin,message_info_end};

		case StatusIcon::WARNING:
			return {message_warning_begin,message_warning_end};

		case StatusIcon::STOP:
			return {led_stop_begin,led_stop_end};

		case StatusIcon::WAIT:
			return {led_wait_begin,led_wait_end};

		case StatusIcon::READY:
			return {led_ready_begin,led_ready_end};

		case StatusIcon::OFF:
			return {led_off_begin,led_off_end};

		}
	return {nullptr,nullptr};
	}

