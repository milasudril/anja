#ifdef __WAND__
target
	[
	name[localeguard.o] type [object] platform[;Windows]
	]
#endif

#include "localeguard.h"
#include <windows.h>
#include <clocale>
#include <cstring>
#include <cstdlib>

struct CriticalSection:public CRITICAL_SECTION
	{
	CriticalSection()
		{InitializeCriticalSection(this);}
	~CriticalSection()
		{DeleteCriticalSection(this);
	};

static CriticalSection LOCALE_MUTEX;

LocaleGuard::LocaleGuard(const char* locale_new)
	{
	EnterCriticalSection(&LOCALE_MUTEX);
	auto locale_old=setlocale(LC_ALL,NULL);
	m_locale_old=strdup(locale_old);
	setlocale(LC_ALL,locale_new);
	}

LocaleGuard::~LocaleGuard()
	{
	setlocale(LC_ALL,m_locale_old);
	free(m_locale_old);
	LeaveCriticalSection(&LOCALE_MUTEX);
	}
