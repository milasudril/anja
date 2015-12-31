#ifdef __WAND__
target[name[audioconnection.h] type[include]]
dependency[audioconnection.o]
#endif

#ifndef AUDIOCONNECTION_H
#define AUDIOCONNECTION_H

#include <cstdint>

class AudioConnection
	{
	public:
		static AudioConnection* create(const char* name);
		virtual void destroy()=0;
		virtual void eventPost(uint8_t slot,uint8_t flags)=0;

	protected:
		virtual ~AudioConnection()=default;

	private:
	};

#endif
