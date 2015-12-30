#ifdef __WAND__
target
	[
	name[localeguard.o] type [object] platform[;GNU/Linux]
	dependency[pthread;external]
	]
#endif

#include "localeguard.h"
#include <clocale>
#include <cstring>
#include <cstdlib>
#include <pthread.h>

static pthread_mutex_t LOCALE_MUTEX=PTHREAD_MUTEX_INITIALIZER;

LocaleGuard::LocaleGuard(const char* locale_new)
	{
	pthread_mutex_lock(&LOCALE_MUTEX);
	auto locale_old=setlocale(LC_ALL,NULL);
	m_locale_old=strdup(locale_old);
	setlocale(LC_ALL,locale_new);
	}

LocaleGuard::~LocaleGuard()
	{
	setlocale(LC_ALL,m_locale_old);
	free(m_locale_old);
	pthread_mutex_unlock(&LOCALE_MUTEX);
	}
