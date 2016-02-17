#ifdef __WAND__
target[name[wavefilewriter.h] type[include]]
dependency[wavefilewriter.o]
#endif

#ifndef WAVEFILEWRITER_H
#define WAVEFILEWRITER_H

#include <memory>

class WavefileInfo;

class WavefileWriter
	{
	private:
		static void destroy(WavefileWriter* obj);

		static WavefileWriter* instanceCreate(const char* path,const WavefileInfo& info);

	public:
		static std::unique_ptr<WavefileWriter,decltype(&destroy)>
		create(const char* path,WavefileInfo& info)
			{return {instanceCreate(path,info),destroy};}

		virtual unsigned int dataWrite(const float* buffer, unsigned int n_frames)=0;
		virtual ~WavefileWriter()=default;
	};

#endif
