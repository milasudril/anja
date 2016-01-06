#ifdef __WAND__
target[name[audioconnection.h] type[include]]
dependency[audioconnection.o]
#endif

#ifndef AUDIOCONNECTION_H
#define AUDIOCONNECTION_H

#include <cstdint>
#include <utility>

class Session;

class AudioConnection
	{
	public:
		class AudioEngine
			{
			public:
				virtual void audioProcess(AudioConnection& source,unsigned int n_frames)=0;

				virtual void buffersAllocate(AudioConnection& source,unsigned int n_frames)
					{}

				virtual void buffersFree()
					{}
			};

		static AudioConnection* create(const char* name);

		virtual void destroy()=0;

		static constexpr uint8_t ACTION_PLAY=0x90;
		static constexpr uint8_t ACTION_STOP=0x80;

		virtual void eventPost(Session& session,uint8_t slot,uint8_t action)=0;

		virtual const float* audioBufferInputGet(unsigned int index,unsigned int n_frames) const=0;
		virtual AudioConnection& audioPortInputAdd(const char* name)=0;
		virtual unsigned int audioPortsInputCount() const=0;

		virtual float* audioBufferOutputGet(unsigned int index,unsigned int n_frames) const=0;
		virtual AudioConnection& audioPortOutputAdd(const char* name)=0;
		virtual unsigned int audioPortsOutputCount() const=0;

		virtual void activate()=0;
		virtual void deactivate()=0;


	protected:
		virtual ~AudioConnection()=default;

	private:
	};

#endif
