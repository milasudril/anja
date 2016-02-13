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

		const ArraySimple<float>& bufferGet() const;

		void readySet() noexcept;

	private:
		Event* m_ready;
		ArraySimple<float> m_data[2];
		unsigned int m_buffer_current;
		unsigned int m_n_written;
	};

#endif
