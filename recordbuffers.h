#ifdef __WAND__
target[name[recordbuffers.h] type[include]]
dependency[recordbuffers.o]
#endif

#ifndef RECORDBUFFERS_H
#define RECORDBUFFERS_H

#include "framework/array_simple.h"

class Event;

class RecordBuffers
	{
	public:

		RecordBuffers(unsigned int n_frames);
		~RecordBuffers();

		void write(const float* value,unsigned int n)noexcept;

		struct WaveformBuffer
			{
			ArraySimple<float> m_data;
			unsigned int m_n_written;
			};
		const WaveformBuffer& bufferGet() const;

		void readySet() noexcept;

		void reset() noexcept
			{
			m_data[0].m_n_written=0;
			m_data[1].m_n_written=0;
			}

	private:
		Event* m_ready;
		WaveformBuffer m_data[2];
		unsigned int m_buffer_current;
		unsigned int m_buffer_current_out;
	};

#endif
