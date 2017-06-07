//@	{
//@	"targets":[{"name":"sessionfilereader.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sessionfilereader-new.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SESSIONFILEREADER_HPP
#define ANJA_SESSIONFILEREADER_HPP

#include <memory>

namespace Anja
	{
	class SessionFileRecord;

	class SessionFileReader
		{
		public:
			explicit SessionFileReader(const char* filename);
			~SessionFileReader();
			bool recordNextGet(SessionFileRecord& record);
			static bool check(const char* filenmae);

		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}
#endif
