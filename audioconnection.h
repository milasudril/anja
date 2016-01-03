#ifdef __WAND__
target[name[audioconnection.h] type[include]]
dependency[audioconnection.o]
#endif

#ifndef AUDIOCONNECTION_H
#define AUDIOCONNECTION_H

#include <cstdint>

class Session;

class AudioConnection
	{
	public:
		static AudioConnection* create(const char* name);
		virtual void destroy()=0;

		static constexpr uint8_t ACTION_PLAY=0x90;
		static constexpr uint8_t ACTION_STOP=0x80;

		virtual void eventPost(Session& session,uint8_t slot,uint8_t action)=0;

	protected:
		virtual ~AudioConnection()=default;

	private:
	};

#endif
