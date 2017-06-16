//@ [
//@		[
//@		"and(gnu,not(less_than(linux,version('2.6.0'))))"
//@			,{"targets":[{"name":"localeguard.o","type":"object"}]}
//@		]
//@	]


#include "localeguard.hpp"
#include <clocale>
#include <cstring>
#include <cstdlib>
#include <pthread.h>

using namespace Anja;

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
