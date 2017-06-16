//@	{
//@	 "targets":[{"name":"statusicons.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"statusicons.o","rel":"implementation"}]
//@	}

#ifndef ANJA_STATUSICONS_HPP
#define ANJA_STATUSICONS_HPP

#include <cstddef>
#include <utility>
#include <cstdint>

namespace Anja
	{
	enum class StatusIcon:size_t{INFORMATION,WARNING,ERROR,READY,WAIT,STOP,OFF};
	constexpr auto StatusIconEnd=static_cast<size_t>( StatusIcon::STOP ) + 1;

	std::pair<const uint8_t*,const uint8_t*> statusIcon(StatusIcon id) noexcept;
	}

#endif
