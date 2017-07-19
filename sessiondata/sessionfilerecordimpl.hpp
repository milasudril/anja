//@	{
//@	 "targets":[{"name":"sessionfilerecordimpl.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sessionfilerecordimpl.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SESSIONFILERECORDIMPL_HPP
#define ANJA_SESSIONFILERECORDIMPL_HPP

#include "sessionfilerecord.hpp"
#include "../common/string.hpp"
#include "../common/range.hpp"
#include <map>

namespace Anja
	{
	class SessionFileRecordImpl final:public SessionFileRecord
		{
		public:
			SessionFileRecordImpl();
			~SessionFileRecordImpl();

			void clear();

			void titleSet(const String& title)
				{m_title=title;}

			const String& titleGet() const
				{return m_title;}

			void levelSet(uint32_t level)
				{m_level=std::min(level,1u);}

			uint32_t levelGet() const
				{return m_level;}

			const String* propertyGet(const String& name) const;

			bool propertySet(const String& name
				,const String& value);

			void propertyReplace(const String& name
				,const String& value);

		private:
			String m_title;
			uint32_t m_level;
			std::map<String,String
				,decltype(&compareLexicographical< String >)> m_props;

			bool propertiesEnumImpl(PropertyEnumeratorFunc enumerator,void* cb_obj) const;
		};
	}
#endif
