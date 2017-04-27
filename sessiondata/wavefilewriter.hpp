//@	{
//@	 "targets":[{"name":"wavefilewriter.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"wavefilewriter.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFILEWRITER_HPP
#define ANJA_WAVEFILEWRITER_HPP

#include <memory>

namespace Anja
	{
	class WavefileInfo;

	class WavefileWriter
		{
		public:
			WavefileWriter(const char* path,WavefileInfo& info);
			~WavefileWriter();
				
			unsigned int dataWrite(const float* buffer, unsigned int n_frames);
		
		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

#endif
