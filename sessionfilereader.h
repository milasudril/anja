#ifdef __WAND__
target[name[sessionfilereader.h] type[include]]
dependency[sessionfilereader.o]
#endif

#ifndef SESSIONFILEREADER_H
#define SESSIONFILEREADER_H

#include <memory>

class SessionFileRecord;

class SessionFileReader
	{
	private:
		typedef void (*Destructor)(SessionFileReader* reader);

	public:
		static std::unique_ptr<SessionFileReader,Destructor>
			create(const char* filename)
			{return {instanceCreate(filename),destroy};}

		virtual bool recordNextGet(SessionFileRecord& record)=0;


	protected:
		virtual ~SessionFileReader()=default;

	private:
		static void destroy(SessionFileReader* reader);
		static SessionFileReader* instanceCreate(const char*filename);
	};

#endif
