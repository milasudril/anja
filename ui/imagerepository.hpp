//@	{
//@	 "targets":[{"name":"imagerepository.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"imagerepository.o","rel":"implementation"}]
//@	}

#ifndef ANJA_IMAGEREPOSITORY_HPP
#define ANJA_IMAGEREPOSITORY_HPP

#include <cstdint>
#include <cstddef>
#include <utility>

namespace Anja
	{
	class ImageRepository
		{
		public:
			ImageRepository();
			~ImageRepository();

			ImageRepository& operator=(ImageRepository&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			ImageRepository(ImageRepository&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			typedef size_t IdType;

			const void* getFromPng(IdType id,const std::pair<const uint8_t*,const uint8_t*>& data) const;

		private:
			class Impl;
			Impl* m_impl;
		};
	}

#endif // ANJA_IMAGEREPOSITORY_HPP
