//@	{
//@	 "targets":[{"name":"imagelist.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"imagelist.o","rel":"implementation"}]
//@	}

#ifndef ANJA_IMAGELIST_HPP
#define ANJA_IMAGELIST_HPP

#include "scrolledwindow.hpp"
#include "box.hpp"
#include "imageview.hpp"
#include "separator.hpp"
#include <vector>
#include <algorithm>

namespace Anja
	{
	class ImageList
		{
		public:
			ImageList& operator=(ImageList&&)=delete;
			ImageList(ImageList&&)=delete;

			explicit ImageList(Container& cnt,bool vertical):
				m_box(cnt,vertical,0)
				{
				m_vertical=vertical;
				m_box.insertMode({0,0});
				}

			ImageList& clear() noexcept
				{
				m_images.clear();
				return *this;
				}

			std::vector<ImageView>::const_iterator begin() const noexcept
				{return m_images.begin();}

			std::vector<ImageView>::const_iterator end() const noexcept
				{return m_images.end();}

			std::vector<ImageView>::iterator begin() noexcept
				{return m_images.begin();}

			std::vector<ImageView>::iterator end() noexcept
				{return m_images.end();}

			ImageView& operator[](int k) noexcept
				{return m_images[k];}

			ImageList& append()
				{
				m_images.push_back(ImageView(m_box));
				return *this;
				}

			template<size_t N>
			ImageList& append()
				{
				for(size_t k=0;k<N;++k)
					{m_images.push_back(ImageView(m_box));}
				return *this;
				}

			ImageList& separator() noexcept
				{
				m_box.insertMode({4,0});
				m_separators.push_back(Separator(m_box,!m_vertical));
				m_box.insertMode({0,0});
				return *this;
				}

			ImageView& back() noexcept
				{return m_images.back();}

			size_t size() const noexcept
				{return m_images.size();}

			ImageList& selected(uint64_t mask);

			template<class Callback,class IdType>
			ImageList& callback(Callback& cb,IdType id) noexcept
				{
				m_id=static_cast<int>( id );
				m_cb=[](void* cb_obj,ImageList& self,int id,ImageView& option)
					{reinterpret_cast<Callback*>(cb_obj)->clicked(self,static_cast<IdType>(id),option);};
				r_cb_obj=&cb;
				callbacks_assign();
				return *this;
				}

			void clicked(ImageView& img,int id)
				{m_cb(r_cb_obj,*this,m_id,img);}

		private:
			int m_id;
			void (*m_cb)(void* cb_obj,ImageList& self,int id,ImageView& option);
			void* r_cb_obj;

			Box m_box;

			std::vector<ImageView> m_images;
			std::vector<Separator> m_separators;
			bool m_vertical;

			void callbacks_assign();
		};
	}

#endif // ANJA_IMAGELIST_HPP
