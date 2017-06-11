//@	{
//@	 "targets":[{"name":"statusicons.o","type":"object"}]
//@	}

#include "statusicons.hpp"

using namespace Anja;

#include "../common/blob.hpp"
#include <maike/targetinclude.hpp>

ANJA_BLOB(uint8_t,message_error,MAIKE_TARGET(message_error.png));
ANJA_BLOB(uint8_t,message_info,MAIKE_TARGET(message_info.png));
ANJA_BLOB(uint8_t,message_warning,MAIKE_TARGET(message_warning.png));

ANJA_BLOB(uint8_t,led_stop,MAIKE_TARGET(led_stop.png));
ANJA_BLOB(uint8_t,led_wait,MAIKE_TARGET(led_wait.png));
ANJA_BLOB(uint8_t,led_ready,MAIKE_TARGET(led_ready.png));

std::pair<const uint8_t*,const uint8_t*> Anja::statusIcon(StatusIcon id) noexcept
	{
	switch(id)
		{
		case StatusIcon::ERROR:
			return {message_error_begin,message_error_end};

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

		}
	return {nullptr,nullptr};
	}

