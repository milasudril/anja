//@	 {"targets":[{"name":"sessionfilerecord.hpp","type":"include"}]}

#ifndef ANJA_SESSIONFILERECORD_HPP
#define ANJA_SESSIONFILERECORD_HPP

#include <cstdint>

namespace Anja
	{
	class String;

	class SessionFileRecord
		{
		public:
			virtual void clear()=0;

			virtual void titleSet(const String& title)=0;

			virtual const String& titleGet() const=0;

			virtual void levelSet(uint32_t level)=0;

			virtual uint32_t levelGet() const=0;

			virtual const String* propertyGet(const String& name) const=0;

			virtual bool propertySet(const String& name
				,const String& property)=0;

			virtual void propertyReplace(const String& name
				,const String& property)=0;

			template<class PropertyEnumerator>
			bool propertiesEnum(PropertyEnumerator&& cb) const
				{
				auto cb_func=[](void* cb_obj
					,const String& name
					,const String& value)
					{
					auto obj=reinterpret_cast<PropertyEnumerator*>(cb_obj);
					return (*obj)(name,value);
					};
				return propertiesEnumImpl(cb_func,&cb);
				}

		protected:
			typedef bool (*PropertyEnumeratorFunc)(void* cb_obj
				,const String& name
				,const String& value);
			~SessionFileRecord()=default;

		private:
			virtual bool propertiesEnumImpl(PropertyEnumeratorFunc enumerator,void* cb_obj) const=0;
		};
	}

#endif
