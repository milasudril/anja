#ifdef __WAND__
target[name[localeguard.h] type[include]]
dependency[localeguard.o]
#endif

#ifndef LOCALEGUARD_H
#define LOCALEGUARD_H

class LocaleGuard
	{
	public:
		LocaleGuard(const char* locale_new);
		~LocaleGuard();

	private:
		char* m_locale_old;
	};

#endif
