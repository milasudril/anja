#ifdef __WAND__
target[name[sessionfilerecordimpl.h] type[include]]
dependency[sessionfilerecordimpl.o]
#endif

#ifndef SESSIONFILERECORDIMPL_H
#define SESSIONFILERECORDIMPL_H

#include "sessionfilerecord.h"
#include "framework/range.h"
#include "framework/arraydynamicshort.h"
#include <map>

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

		void propertiesEnum(PropertyEnumerator& enumerator) const;

	private:
		ArrayDynamicShort<char> m_title;
		uint32_t m_level;
		std::map<ArrayDynamicShort<char>,ArrayDynamicShort<char>
			,decltype(&compareLexicographical< ArrayDynamicShort<char> >)> m_props;
	};

#endif
