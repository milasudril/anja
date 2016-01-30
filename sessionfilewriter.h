#ifdef __WAND__
target[name[sessionfilewriter.h] type[include]]
dependency[sessionfilewriter.o]
#endif

#ifndef SESSIONFILEWRITER_H
#define SESSIONFILEWRITER_H

#include <memory>

class SessionFileRecord;

class SessionFileWriter
	{
	private:
		typedef void (*Destructor)(SessionFileWriter* reader);

	public:
		static std::unique_ptr<SessionFileWriter,Destructor>
			create(const char* filename)
			{return {instanceCreate(filename),destroy};}

		virtual void recordWrite(const SessionFileRecord& record)=0;

	protected:
		virtual ~SessionFileWriter()=default;

	private:
		static void destroy(SessionFileWriter* reader);
		static SessionFileWriter* instanceCreate(const char*filename);
	};

#endif
