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

			void sectionTitleSet(const ArrayDynamicShort<char>& title)
				{m_title=title;}

			const ArrayDynamicShort<char>& titleGet() const
				{return m_title;}

			void sectionLevelSet(uint32_t level)
				{m_level=std::min(level,1u);}

			uint32_t sectionLevelGet() const
				{return m_level;}

			const ArrayDynamicShort<char>* propertyGet(const ArrayDynamicShort<char>& name) const;

			bool propertySet(const ArrayDynamicShort<char>& name
				,const ArrayDynamicShort<char>& value);

			void propertyReplace(const ArrayDynamicShort<char>& name
				,const ArrayDynamicShort<char>& value);

			bool propertiesEnum(PropertyEnumeratorFunc enumerator,void* cb_obj) const;

		private:
			ArrayDynamicShort<char> m_title;
			uint32_t m_level;
			std::map<ArrayDynamicShort<char>,ArrayDynamicShort<char>
				,decltype(&compareLexicographical< ArrayDynamicShort<char> >)> m_props;
		};
	}
#endif
