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
				virtual void audioProcess(AudioConnection& source,unsigned int n_frames) noexcept=0;

				virtual void onActivate(AudioConnection& source)
					{}

				virtual void onDeactivate(AudioConnection& source)
					{}

				virtual void buffersAllocate(AudioConnection& source,unsigned int n_frames)
					{}
			};

		static AudioConnection* create(const char* name,AudioEngine& engine);

		virtual void destroy()=0;

		virtual const float* audioBufferInputGet(unsigned int index,unsigned int n_frames) const noexcept=0;
		virtual AudioConnection& audioPortInputAdd(const char* name)=0;
		virtual unsigned int audioPortsInputCount() const noexcept=0;

		virtual float* audioBufferOutputGet(unsigned int index,unsigned int n_frames) const noexcept=0;
		virtual AudioConnection& audioPortOutputAdd(const char* name)=0;
		virtual unsigned int audioPortsOutputCount() const noexcept=0;

		struct MIDIEvent
			{
			uint32_t time_offset;
			uint8_t data[4];
			};

		virtual bool midiEventGet(unsigned int port,unsigned int index
			,MIDIEvent& event) noexcept=0;
		virtual AudioConnection& midiPortInputAdd(const char* name)=0;
		virtual unsigned int midiPortsInputCount() const noexcept=0;

		struct MIDIBufferOutputHandle
			{
			void* buffer;
			};

		virtual void midiEventWrite(MIDIBufferOutputHandle handle,const MIDIEvent& event) noexcept=0;
		virtual MIDIBufferOutputHandle midiBufferOutputGet(unsigned int port,unsigned int n_frames) noexcept=0;
		virtual AudioConnection& midiPortOutputAdd(const char* name)=0;
		virtual unsigned int midiPortsOutputCount() const noexcept=0;

		virtual void activate()=0;
		virtual void deactivate()=0;

		virtual double sampleRateGet() const noexcept=0;


	protected:
		virtual ~AudioConnection()=default;

	private:
	};

#endif
