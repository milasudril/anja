//@	{
//@	 "targets":[{"name":"imagerepository.o","type":"object"
//@		,"pkgconfig_libs":["cairo"]}]
//@	}

#include "imagerepository.hpp"
#include <map>
#include <cairo.h>
#include <cstring>

using namespace Anja;

typedef std::pair<const uint8_t*,const uint8_t*> ReadPair;

static cairo_surface_t* load(ReadPair read_pair)
	{
	auto temp=cairo_image_surface_create_from_png_stream([]
		(void* cb_obj,unsigned char *data,unsigned int length)
		{
		auto rp=reinterpret_cast<ReadPair*>(cb_obj);
		auto n=std::min(static_cast<unsigned int>(rp->second - rp->first),length);
		memcpy(data,rp->first,n);
		rp->first+=n;
		return CAIRO_STATUS_SUCCESS;
		},&read_pair);
	if(cairo_surface_status(temp)!=CAIRO_STATUS_SUCCESS)
		{
		cairo_surface_destroy(temp);
		return nullptr;
		}
	return temp;
	}

class ImageRepository::Impl
	{
	public:
		const void* getFromPng(IdType id
			,const std::pair<const uint8_t*,const uint8_t*>& data) const
			{
			auto i=m_data.find(id);
			if(i==m_data.end())
				{
				return m_data.insert({id,load(data)}).first->second;
				}
			return i->second;
			}

	private:
		mutable std::map<IdType,cairo_surface_t*> m_data;
	};

ImageRepository::ImageRepository()
	{m_impl=new Impl;}

ImageRepository::~ImageRepository()
	{delete m_impl;}

const void* ImageRepository::getFromPng(IdType id
	,const std::pair<const uint8_t*,const uint8_t*>& data) const
	{return m_impl->getFromPng(id,data);}
