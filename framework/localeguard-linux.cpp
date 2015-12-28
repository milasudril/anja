#ifdef __WAND__
target
	[
	name[localeguard.o] type [object] platform[;GNU/Linux]
	dependency[pthread;external]
	]
#endif

#include "localeguard.h"
#include <clocale>
#include <pthread.h>

static pthread_mutex_t LOCALE_MUTEX=PTHREAD_MUTEX_INITIALIZER;

LocaleGuard::LocaleGuard(const char* locale_new)
	{
	pthread_mutex_lock(&LOCALE_MUTEX);
	r_locale_old=setlocale(LC_ALL,locale_new);
	}

LocaleGuard::~LocaleGuard()
	{
	setlocale(LC_ALL,r_locale_old);
	pthread_mutex_unlock(&LOCALE_MUTEX);
	}
