//@	{
//@	 "targets":[{"name":"sessionfilerecordimpl.o","type":"object"}]
//@	}

#include "sessionfilerecordimpl.hpp"

using namespace Anja;

SessionFileRecordImpl::SessionFileRecordImpl():
	m_props(compareLexicographical< String >)
	{}

SessionFileRecordImpl::~SessionFileRecordImpl(){}

void SessionFileRecordImpl::clear()
	{
	m_props.clear();
	}

const String*
SessionFileRecordImpl::propertyGet(const String& name) const
	{
	auto i=m_props.find(name);
	if(i==m_props.end())
		{return nullptr;}
	return &(i->second);
	}

bool SessionFileRecordImpl::propertySet(const String& name
	,const String& value)
	{
	return m_props.insert({name,value}).second;
	}

void SessionFileRecordImpl::propertyReplace(const String& name
	,const String& value)
	{
	m_props[name]=value;
	}

bool SessionFileRecordImpl::propertiesEnum(PropertyEnumeratorFunc enumerator,void* cb_obj) const
	{
	auto i=m_props.begin();
	auto i_end=m_props.end();
	while(i!=i_end)
		{
		if(!enumerator(cb_obj,i->first,i->second))
			{return false;}
		++i;
		}
	return false;
	}
