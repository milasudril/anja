#ifdef __WAND__
target[name[playbackrange.h] type[include]]
dependency[playbackrange.o]
#endif

#ifndef PLAYBACKRANGE_H
#define PLAYBACKRANGE_H

class Waveform;

class alignas(32) PlaybackRange
	{
	public:
		PlaybackRange(const Waveform& waveform);

		unsigned int outputBufferGenerate(float* buffer_out
			,unsigned int n_frames_out,double fs_out);

	private:
		const float* r_begin;
		const float* r_current;
		const float* r_end;
		float m_fs;
		float m_gain;
	};

#endif

