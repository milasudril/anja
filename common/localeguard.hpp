//@	{
//@	 "targets":[{"name":"localeguard.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"localeguard.o","rel":"implementation"}]
//@	}

#ifndef ANJA_LOCALEGUARD_HPP
#define ANJA_LOCALEGUARD_HPP

namespace Anja
	{
	class LocaleGuard
		{
		public:
			LocaleGuard(const char* locale_new);
			~LocaleGuard();

		private:
			char* m_locale_old;
		};
	}

#endif
