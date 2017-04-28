#ifdef __WAND__
target[name[sessionfilerecord.h] type[include]]
#endif

#ifndef SESSIONFILERECORD_H
#define SESSIONFILERECORD_H

#include <cstdint>

template<class T>
class ArrayDynamicShort;

class SessionFileRecord
	{
	public:
		virtual void clear()=0;

		virtual void sectionTitleSet(const ArrayDynamicShort<char>& title)=0;

		virtual const ArrayDynamicShort<char>& titleGet() const=0;

		virtual void sectionLevelSet(uint32_t level)=0;

		virtual uint32_t sectionLevelGet() const=0;

		virtual const ArrayDynamicShort<char>* propertyGet(const ArrayDynamicShort<char>& name) const=0;

		virtual bool propertySet(const ArrayDynamicShort<char>& name
			,const ArrayDynamicShort<char>& property)=0;

		virtual void propertyReplace(const ArrayDynamicShort<char>& name
			,const ArrayDynamicShort<char>& property)=0;

		class PropertyEnumerator
			{
			public:
				virtual void propertyVisit(const ArrayDynamicShort<char>& name
					,const ArrayDynamicShort<char>& value)=0;
			};

		virtual void propertiesEnum(PropertyEnumerator&& enumerator) const=0;

		virtual ~SessionFileRecord()=default;
	};

#endif
