//@	{
//@	 "targets":[{"name":"wavefilereader.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"wavefilereader.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFILEREADER_HPP
#define ANJA_WAVEFILEREADER_HPP

#include <memory>

namespace Anja
	{
	class WavefileInfo;

	class WavefileReader
		{
		public:
			WavefileReader(const char* path,WavefileInfo& info);
			~WavefileReader();

			unsigned int dataRread(float* buffer,unsigned int n_frames);

		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

#endif
