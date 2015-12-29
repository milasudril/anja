#ifdef __WAND__
target[name[wavefilereader.h] type[include]]
dependency[wavefilereader.o]
#endif

#ifndef WAVEFILELOADER_H
#define WAVEFILELOADER_H

#include <memory>

class WavefileInfo;

class WavefileReader
	{
	private:
		static void destroy(WavefileReader* obj);

		static WavefileReader* instanceCreate(const char* path,WavefileInfo& info);

	public:
		static std::unique_ptr<WavefileReader,decltype(&destroy)>
		create(const char* path,WavefileInfo& info)
			{return {instanceCreate(path,info),destroy};}

		virtual unsigned int dataRead(float* buffer, unsigned int n_frames)=0;
		virtual ~WavefileReader()=default;
	};

#endif
