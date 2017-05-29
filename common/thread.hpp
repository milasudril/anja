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
	class Thread
		{
		public:
			Thread(const Thread&)=delete;
			Thread& operator=(const Thread&)=delete;

			template<class Callback,class TaskId>
			explicit Thread(Callback& cb,TaskId id):m_cb(thread_entry<Callback,TaskId>)
				,r_cb_obj(&cb),m_id(static_cast<int>(id))
				{run();}

			~Thread();

		private:
			template<class Callback,class TaskId>
			static void thread_entry(void* obj,int id)
				{reinterpret_cast<Callback*>(obj)->run(static_cast<TaskId>(id));}

		#ifdef _WIN32
			static unsigned int __stdcall thread_entry_system(void* thread);
		#else
			static void* thread_entry_system(void* thread);
		#endif

			void run();

			typedef void (*Callback)(void* cb_obj,int id);
			Callback m_cb;
			void* r_cb_obj;
			int m_id;
			intptr_t m_handle;
		};
	}

#endif
