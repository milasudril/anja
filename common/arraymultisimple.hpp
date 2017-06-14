//@	 {"targets":[{"name":"arraymultisimple.hpp","type":"include"}]}

#ifndef ANJA_ARRAYMULTISIMPLE_HPP
#define ANJA_ARRAYMULTISIMPLE_HPP

#include "vectortype.hpp"
#include "memoryalloc.hpp"
#include "arrayinit.hpp"
#include <cstddef>
#include <utility>
#include <algorithm>
#include <cassert>
#include <tuple>

namespace Anja
	{
	template<class ... T>
	class ArrayMultiSimple
		{
		public:
			typedef size_t SizeType;

			~ArrayMultiSimple();

			explicit ArrayMultiSimple(SizeType n_elems);

			template<class Initializer>
			explicit ArrayMultiSimple(SizeType n_elems,Initializer&& initializer);


			ArrayMultiSimple(const ArrayMultiSimple& a);

			ArrayMultiSimple& operator=(const ArrayMultiSimple& a)
				{
				assert( &a != this);
				ArrayMultiSimple<T...> temp(a);
				*this=std::move(temp);
				return *this;
				}

			ArrayMultiSimple(ArrayMultiSimple&& a) noexcept
				{
				m_N=a.m_N;
				m_data=a.m_data;
				a.m_N=0;
				}

			ArrayMultiSimple& operator=(ArrayMultiSimple&& a) noexcept
				{
 				assert(&a != this);
				std::swap(m_N,a.m_N);
				std::swap(m_data,a.m_data);
				return *this;
				}

			template<size_t ArrayIndex>
			auto begin() noexcept
				{return std::get<ArrayIndex>(m_data);}


			template<size_t ArrayIndex>
			auto begin() const noexcept
				{return std::get<ArrayIndex>(m_data);}

			template<size_t ArrayIndex>
			auto end() noexcept
				{return std::get<ArrayIndex>(m_data) + m_N;}

			template<size_t ArrayIndex>
			auto end() const noexcept
				{return std::get<ArrayIndex>(m_data) + m_N;}


			SizeType length() const noexcept
				{return m_N;}

			template<size_t ArrayIndex>
			auto& get(size_t index) noexcept
				{
				assert(index<m_N);
				return *(begin<ArrayIndex>() + index);
				}

			template<size_t ArrayIndex>
			const auto& get(size_t index) const noexcept
				{
				assert(index<m_N);
				return *(begin<ArrayIndex>() + index);
				}


		private:
			size_t m_N;
			std::tuple<T*...> m_data;
		};

	template<std::size_t I = 0, typename FuncT, typename... Tp>
	inline typename std::enable_if<I == sizeof...(Tp), void>::type
	for_each(std::tuple<Tp...> &, FuncT) // Unused arguments are given no names.
		{}

	template<std::size_t I = 0, typename FuncT, typename... Tp>
	inline typename std::enable_if<I < sizeof...(Tp), void>::type
	for_each(std::tuple<Tp...>& t, FuncT f)
		{
		f(std::get<I>(t),std::integral_constant<decltype(I),I>{});
		for_each<I + 1, FuncT, Tp...>(t, f);
		}


	template<class ...T>
	std::tuple<T*...> alloc(size_t N)
		{
		std::tuple<T*...> ret;
		int success_count=0;
		try
			{
			for_each(ret,[N,&success_count](auto& x,auto index)
				{
				auto tmp=memoryAllocate(N*sizeof(*x));
				x=static_cast<std::remove_reference_t<decltype(x)> >(tmp);
				++success_count;
				});
			}
		catch(...)
			{
			int k=0;
			for_each(ret,[success_count,&k](auto x,auto index)
				{
				if(k<success_count)
					{
					memoryFree(x);
					++k;
					}
				});
			}
		return ret;
		}

	template<class ... T>
	ArrayMultiSimple<T...>::ArrayMultiSimple(SizeType n_elems)
		{
		assert(n_elems!=0);
		auto data=alloc<T...>(n_elems);
		int success_count=0;
		try
			{
			for_each(data,[n_elems,&success_count](auto& x,auto index)
				{
				ArrayInit::create(x,x+n_elems);
				++success_count;
				});
			}
		catch(...)
			{
			int k=0;
			for_each(data,[success_count,n_elems,&k](auto x,auto index)
				{
				if(k<success_count)
					{
					ArrayInit::destroy(x,x+n_elems);
					++k;
					}
				memoryFree(x);
				});
			throw;
			}
		m_N=n_elems;
		m_data=data;
		}

	template<class ... T>
	ArrayMultiSimple<T...>::~ArrayMultiSimple()
		{
		for_each(m_data,[this](auto x,auto index)
			{
			ArrayInit::destroy(x,x+m_N);
			memoryFree(x);
			});
		}


	template<class ... T>
	ArrayMultiSimple<T...>::ArrayMultiSimple(const ArrayMultiSimple& a)
		{
		assert(a.begin()!=nullptr);
		auto N=a.length();
		auto data=alloc(N);
		int success_count=0;
		try
			{
			for_each(data,[N,&a,&success_count](auto x,auto index)
				{
				ArrayInit::copy(x,x+N,a.begin<decltype(index)::value>());
				++success_count;
				});
			}
		catch(...)
			{
			int k=0;

			for_each(data,[success_count,N,&k](auto x)
				{
				if(k<success_count)
					{
					ArrayInit::destroy(x,x+N);
					++k;
					}
				memoryFree(x);
				});
			throw;
			}

		m_N=N;
		m_data=data;
		}
	}
#endif
