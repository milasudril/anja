//@	{
//@	 "targets":[{"name":"sessionfilerecordimpl.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sessionfilerecordimpl.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SESSIONFILERECORDIMPL_HPP
#define ANJA_SESSIONFILERECORDIMPL_HPP

#include "sessionfilerecord.hpp"
#include "../common/arraydynamicshort.hpp"
#include "../common/range.hpp"
#include <map>

namespace Anja
	{
	class SessionFileRecordImpl:public SessionFileRecord
		{
		public:
			SessionFileRecordImpl();
			~SessionFileRecordImpl();

			void clear();

			void sectionTitleSet(const String& title)
				{m_title=title;}

			const String& titleGet() const
				{return m_title;}

			void sectionLevelSet(uint32_t level)
				{m_level=std::min(level,1u);}

			uint32_t sectionLevelGet() const
				{return m_level;}

			const String* propertyGet(const String& name) const;

			bool propertySet(const String& name
				,const String& value);

			void propertyReplace(const String& name
				,const String& value);

			bool propertiesEnum(PropertyEnumeratorFunc enumerator,void* cb_obj) const;

		private:
			String m_title;
			uint32_t m_level;
			std::map<String,String
				,decltype(&compareLexicographical< String >)> m_props;
		};
	}
#endif
