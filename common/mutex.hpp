//@	{
//@	 "dependencies_extra":[{"ref":"mutex.o","rel":"implementation"}]
//@	,"targets":[{"name":"mutex.h","type":"include"}]
//@	}

#ifndef ANJA_MUTEX_HPP
#define ANJA_MUTEX_HPP

#include <cstdint>

namespace Anja
	{
	class Mutex
		{
		public:
			class LockGuard
				{
				public:
					LockGuard(const LockGuard&)=delete;
					LockGuard& operator=(const LockGuard&)=delete;
					explicit LockGuard(Mutex& m):r_m(m)
						{m.lock();}

					~LockGuard()
						{r_m.unlock();}
				private:
					Mutex& r_m;
				};

			Mutex();
			~Mutex();

			void lock() noexcept;
			void unlock() noexcept;
			bool lockTry() noexcept;

		private:
#if (__amd64 || __x86_64 || __x86_64__ || __amd64__)
			static constexpr int MUTEX_SIZE=40;
#else
			static constexpr int MUTEX_SIZE=24;
#endif
			union
				{
				double dummy[MUTEX_SIZE/sizeof(double)];
				uint8_t data[MUTEX_SIZE];
				} m_impl;
		};
	}

#endif
