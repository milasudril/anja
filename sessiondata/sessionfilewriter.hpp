//@	{
//@	"targets":[{"name":"sessionfilewriter.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sessionfilewriter.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SESSIONFILEWRITER_HPP
#define ANJA_SESSIONFILEWRITER_HPP

#include <memory>

namespace Anja
	{
	class SessionFileRecord;

	class SessionFileWriter
		{
		public:
			explicit SessionFileWriter(const char* filename);
			~SessionFileWriter();
			void recordWrite(const SessionFileRecord& record);

		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}
#endif
