//@	{
//@	"targets":[{"name":"syserror.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"syserror.o","rel":"implementation"}]
//@}

#ifndef ANJA_SYSERROR_HPP
#define ANJA_SYSERROR_HPP

namespace Anja
	{
	class SysError
		{
		public:
			explicit SysError(int err) noexcept:m_err(err)
				{}

			int status() const noexcept
				{return m_err;}

		private:
			int m_err;
		};
	};

#endif
