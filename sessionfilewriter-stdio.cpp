#ifdef __WAND__
target[name[sessionfilewriter.o] type[object]]
#endif

#include "sessionfilewriter.h"
#include "sessionfilerecord.h"
#include "framework/arraydynamicshort.h"
#include <cstdint>
#include <cstdio>
#include <cassert>

class SessionFileWriterStdio:public SessionFileWriter
	{
	public:
		SessionFileWriterStdio(const char* filename);
		~SessionFileWriterStdio();

		void recordWrite(const SessionFileRecord& record);

	private:
		std::unique_ptr<FILE,decltype(&fclose)> m_source;
	};

SessionFileWriter* SessionFileWriter::instanceCreate(const char* filename)
	{
	return new SessionFileWriterStdio(filename);
	}

void SessionFileWriter::destroy(SessionFileWriter* reader)
	{
	delete reinterpret_cast<SessionFileWriterStdio*>(reader);
	}

SessionFileWriterStdio::SessionFileWriterStdio(const char* filename):
	m_source{fopen(filename,"wb"),fclose}
	{
	if(m_source.get()==NULL)
		{
	//	TODO throw something better than "const char*"
		throw "Could not open target session file.";
		}
	}

SessionFileWriterStdio::~SessionFileWriterStdio()
	{
	}

class PropertyEnumerator:public SessionFileRecord::PropertyEnumerator
	{
	public:
		PropertyEnumerator(FILE* file_out):r_file_out(file_out)
			{}

		void propertyVisit(const ArrayDynamicShort<char>& name
			,const ArrayDynamicShort<char>& value)
			{
			fprintf(r_file_out,"%s: %s\n\n",name.begin(),value.begin());
			}
	private:
		FILE* r_file_out;
	};

void SessionFileWriterStdio::recordWrite(const SessionFileRecord& record)
	{
	char marker;
	switch(record.sectionLevelGet())
		{
		case 0:
			marker='=';
			break;
		case 1:
			marker='-';
			break;
		default:
			throw "Invalid section level";
		}

	fprintf(m_source.get(),"%c%s%c\n",marker,record.titleGet().begin(),marker);

	record.propertiesEnum(PropertyEnumerator(m_source.get()));
	}
