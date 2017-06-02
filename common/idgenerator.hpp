//@	{"targets":[{"name":"idgenerator.hpp","type":"include"}]}

#ifndef ANJA_IDGENERATOR_HPP
#define ANJA_IDGENERATOR_HPP

#include <limits>
#include <cassert>
#include <type_traits>

namespace Anja
	{
	template<class Freelist>
	class IdGenerator
		{
		public:
			static constexpr typename Freelist::value_type id_null=-1;

			static constexpr typename Freelist::value_type null() noexcept
				{return id_null;}

			template<class ... FreelistParams>
			IdGenerator(FreelistParams ... x) noexcept:m_freelist(x...),m_id_next(0){}

			auto idGet() noexcept
				{
				if(m_freelist.empty())
					{
					if(static_cast<size_t>(m_id_next)==m_freelist.capacityMax() - 1)
						{return id_null;}
					auto ret=m_id_next;
					++m_id_next;
					return ret;
					}
				return m_freelist.pop_front();
				}

			void fill() noexcept
				{
				while(static_cast<size_t>(m_id_next)!=m_freelist.capacityMax())
					{
					idRelease(m_id_next);
					++m_id_next;
					}
				}

			IdGenerator& idRelease(const typename Freelist::value_type& id)
				{
				assert(m_freelist.length()!=m_freelist.capacityMax());
				m_freelist.push_back(id);
				return *this;
				}

			IdGenerator& reset() noexcept
				{
				m_freelist.clear();
				m_id_next=0;
				return *this;
				}

		private:
			Freelist m_freelist;
			int m_id_next;
		};
	}
#endif
