//@	{
//@	 "targets":[{"name":"thread.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"thread.o","rel":"implementation"}]
//@	}

#ifndef ANJA_THREAD_HPP
#define ANJA_THREAD_HPP

#include <cstdint>
#include <utility>

namespace Anja
	{
	class ThreadBase
		{
		public:
			ThreadBase(const ThreadBase&)=delete;
			ThreadBase& operator=(const ThreadBase&)=delete;

			virtual void run()=0;

		protected:
			ThreadBase();
			virtual ~ThreadBase();

			void start();
			void synchronize() noexcept;

		private:
			intptr_t m_handle;
		};

	template<class Function>
	class Thread final:private ThreadBase
		{
		public:
			explicit Thread(Function&& entry):m_entry(std::move(entry))
				{start();}

			void run()
				{m_entry();}

			~Thread() noexcept
				{synchronize();}

		private:
			Function m_entry;
		};
	}

#endif
