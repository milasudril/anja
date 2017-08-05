//@	{
//@	 "targets":[{"name":"mutex.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"mutex.o","rel":"implementation"}]
//@	}

#ifndef ANJA_MUTEX_HPP
#define ANJA_MUTEX_HPP

#include "error.hpp"
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

			class LockGuardNonblocking
				{
				public:
					LockGuardNonblocking(const LockGuardNonblocking&)=delete;
					LockGuardNonblocking& operator=(const LockGuardNonblocking&)=delete;

					LockGuardNonblocking(LockGuardNonblocking&&)=default;
					LockGuardNonblocking& operator=(LockGuardNonblocking&&)=default;

					explicit LockGuardNonblocking(Mutex& m):r_m(&m)
						{
						if(!m.lockTry())
							{throw Error("The current resource is temporary busy. Please try again later.");}
						}

					~LockGuardNonblocking()
						{r_m->unlock();}
				private:
					Mutex* r_m;
				};

			Mutex();
			Mutex(const Mutex&)=delete;
			Mutex(Mutex&&)=delete;
			Mutex& operator=(const Mutex&)=delete;
			Mutex& operator=(Mutex&&)=delete;

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
