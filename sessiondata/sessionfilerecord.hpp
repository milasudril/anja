//@	 {"targets":[{"name":"sessionfilerecord.hpp","type":"include"}]}

#ifndef ANJA_SESSIONFILERECORD_HPP
#define ANJA_SESSIONFILERECORD_HPP

#include <cstdint>

namespace Anja
	{
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

			template<class PropertyEnumerator>
			bool propertiesEnum(PropertyEnumerator&& cb)
				{
				auto cb_func=[](void* cb_obj
					,const ArrayDynamicShort<char>& name
					,const ArrayDynamicShort<char>& value)
					{
					auto obj=reinterpret_cast<PropertyEnumerator*>(cb_obj);
					return obj->sessionFileRecord(name,value);
					};
				return propertiesEnum(cb_func,&cb);
				}

		protected:
			typedef bool (*PropertyEnumeratorFunc)(void* cb_obj
				,const ArrayDynamicShort<char>& name
				,const ArrayDynamicShort<char>& value);

			virtual bool propertiesEnum(PropertyEnumeratorFunc enumerator,void* cb_obj) const=0;
			~SessionFileRecord()=default;
		};
	}

#endif
