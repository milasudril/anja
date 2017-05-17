//@	{"targets":[{"name":"sessionfilewriter.o","type":"object"}]}

#include "sessionfilewriter.hpp"
#include "sessionfilerecord.hpp"
#include "../common/string.hpp"
#include "../common/error.hpp"
#include <cstdint>
#include <cstdio>
#include <cassert>

using namespace Anja;

class SessionFileWriter::Impl
	{
	public:
		explicit Impl(const char* filename);
		~Impl();

		void recordWrite(const SessionFileRecord& record);

	private:
		std::unique_ptr<FILE,decltype(&fclose)> m_sink;
	};

SessionFileWriter::SessionFileWriter(const char* filename)
	{
	m_impl.reset( new Impl(filename) );
	}
SessionFileWriter::~SessionFileWriter()
	{}

SessionFileWriter::Impl::Impl(const char* filename):
	m_sink{fopen(filename,"wb"),fclose}
	{
	if(m_sink.get()==NULL)
		{throw Error("It was not possible to open the file ",filename,". ",SysError(errno));}
	}

SessionFileWriter::Impl::~Impl()
	{}

void SessionFileWriter::recordWrite(const SessionFileRecord& record)
	{
	m_impl->recordWrite(record);
	}

void SessionFileWriter::Impl::recordWrite(const SessionFileRecord& record)
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
			throw Error("Internal error: Invalid section level.");
		}

	fprintf(m_sink.get(),"%c%s%c\n",marker,record.titleGet().begin(),marker);

	record.propertiesEnum([this](const String& name
		,const String& value)
			{
			fprintf(m_sink.get(),"%s: %s\n\n",name.begin(),value.begin());
			return true;
			});
	}
