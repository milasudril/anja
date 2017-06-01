//@	{"targets":[{"name":"nextpow2.hpp","type":"include"}]}

#ifndef ANJA_NEXTPOW2_HPP
#define ANJA_NEXTPOW2_HPP

#include <climits>
#include <type_traits>

namespace Anja
	{
	template<class UnsignedType>
	class Nextpow2
		{
		public:
			constexpr explicit Nextpow2(UnsignedType val):m_value(nextpow2(val))
				{}

			constexpr operator UnsignedType() const noexcept
				{return m_value;}

			constexpr Nextpow2& operator<<=(UnsignedType n) noexcept
				{
				m_value<<=n;
				return *this;
				}

			constexpr Nextpow2& operator>>=(UnsignedType n) noexcept
				{
				m_value<<=n;
				return *this;
				}

		private:
			static constexpr UnsignedType nextpow2(UnsignedType v)
				{
				//This comes from robson@stackoverflow
				//https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2/27915457#27915457

				static_assert(std::is_unsigned<UnsignedType>::value, "Only works for unsigned types");
				v--;
				for (size_t i = 1; i < sizeof(v) * CHAR_BIT; i *= 2)
					{v|= (v >> i);}
				return ++v;
				}
			static_assert(nextpow2(0u)==0u,"");
			static_assert(nextpow2(1000u)==1024u,"");
			UnsignedType m_value;
		};

	template<class UnsignedType>
	UnsignedType operator%(UnsignedType a,Nextpow2<UnsignedType> b)
		{
		return a&(b.value()-1);
		}

	template<class UnsignedType>
	Nextpow2<UnsignedType> operator<<(Nextpow2<UnsignedType> a,UnsignedType n)
		{return a<<=n;}

	template<class UnsignedType>
	Nextpow2<UnsignedType> operator>>(Nextpow2<UnsignedType> a,UnsignedType n)
		{return a>>=n;}



	template<class UnsignedType>
	bool operator==(Nextpow2<UnsignedType> a,Nextpow2<UnsignedType> b)
		{return UnsignedType(a)==UnsignedType(b);}

	template<class UnsignedType>
	bool operator==(UnsignedType a,Nextpow2<UnsignedType> b)
		{return a==UnsignedType(b);}

	template<class UnsignedType>
	bool operator==(Nextpow2<UnsignedType> a,UnsignedType b)
		{return UnsignedType(a)==b;}


	template<class UnsignedType>
	bool operator!=(Nextpow2<UnsignedType> a,Nextpow2<UnsignedType> b)
		{return !(UnsignedType(a)==UnsignedType(b));}

	template<class UnsignedType>
	bool operator!=(UnsignedType a,Nextpow2<UnsignedType> b)
		{return !(a==UnsignedType(b));}

	template<class UnsignedType>
	bool operator!=(Nextpow2<UnsignedType> a,UnsignedType b)
		{return !(UnsignedType(a)==b);}


	template<class UnsignedType>
	bool operator<(Nextpow2<UnsignedType> a,Nextpow2<UnsignedType> b)
		{return UnsignedType(a)<UnsignedType(b);}

	template<class UnsignedType>
	bool operator<(UnsignedType a,Nextpow2<UnsignedType> b)
		{return a<UnsignedType(b);}

	template<class UnsignedType>
	bool operator<(Nextpow2<UnsignedType> a,UnsignedType b)
		{return UnsignedType(a)==b;}
	}

#endif
