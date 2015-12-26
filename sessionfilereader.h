#ifdef __WAND__
target[name[sessionfilereader.h] type[include]]
dependency[sessionfilereader.o]
#endif

#ifndef SESSIONFILEREADER_H
#define SESSIONFILEREADER_H

#include <memory>

class Session;
class WaveformData;

class SessionFileReader
	{
	private:
		typedef void (*Destructor)(SessionFileReader* reader);

	public:
		static std::unique_ptr<SessionFileReader,Destructor>
			create(const char* filename,Session& session_data)
			{return {instanceCreate(filename,session_data),destroy};}

		virtual bool nextSlotGet(WaveformData& slot)=0;


	protected:
		virtual ~SessionFileReader()=default;

	private:
		static void destroy(SessionFileReader* reader);
		static SessionFileReader* instanceCreate(const char*filename
			,Session& session_data);
	};

#endif
