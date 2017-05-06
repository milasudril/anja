//@	{
//@	 "targets":{"name":"filenameselect.hpp","type":"include"}
//@	,"dependencies_extra":[{"ref":"filenameselect.o","rel":"implementation"}]
//@	}
#ifndef ANJA_FILENAMEPICKER_HPP
#define ANJA_FILENAMEPICKER_HPP

#include "container.hpp"

#include <utility>
#include <string>
#include <memory>

namespace Anja
	{
	enum class FilenameSelectMode:int{OPEN,SAVE};

	typedef bool (*FilenameSelectFilter)(void* cb_obj,const char* filename_check);
	
	bool filenameSelect(const Container& cnt,std::string& filename_in,FilenameSelectMode mode
		,FilenameSelectFilter cb,void* cb_obj,const char* filter_name);

	inline bool filenameSelect(const Container& cnt,std::string& filename_in,FilenameSelectMode mode)
		{
		auto cb=[](void* cb_obj,const char* filename_check)
			{return true;};
		return filenameSelect(cnt,filename_in,mode,cb,nullptr,"");
		}

	template<class FilterCallback>
	inline bool filenameSelect(const Container& cnt,std::string& filename_in,FilenameSelectMode mode
		,FilterCallback&& filter,const char* filter_name)
		{
		auto cb=[](void* cb_obj,const char* filename_check)
			{
			auto filter=reinterpret_cast<FilterCallback*>(cb_obj);
			return (*filter)(filename_check);
			};
		return filenameSelect(cnt,filename_in,mode,cb,&filter,filter_name);
		}


	typedef void (*FilenameSelectCallback)(void* cb_obj,const char* context
		,std::unique_ptr<char*>&& result);

	void filenameSelectRemote(Container& cnt,const char* filename_in
		,FilenameSelectMode mode,const char* context_name,FilenameSelectCallback cb,void* cb_obj);

	template<class Callback>
	void filenameSelectRemote(Container& cnt,const char* filename_in
		,FilenameSelectMode mode,const char* context_name,Callback& cb_obj)
		{
		auto cb=[](void* cb_obj,const char* context,std::unique_ptr<char*>&& result)
			{
			auto callback=reinterpret_cast<Callback*>(cb_obj);
			callback->filenameReceived(context,std::move(result));
			};
		filenameSelectRemote(cnt,filename_in,mode,context_name,cb,&cb_obj);
		}
	}

#endif