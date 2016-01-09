#ifdef __WAND__
target[name[audioengineanja.o] type[object]]
#endif

#include "audioengineanja.h"

AudioEngineAnja::AudioEngineAnja():m_event_queue(32)
	{}

void AudioEngineAnja::eventMIDIPost(uint8_t status,uint8_t value_0,uint8_t value_1)
	{

	}
