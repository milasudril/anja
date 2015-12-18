#ifdef __WAND__
target[name[eventloop.h] type[include]]
dependency[eventloop.o]
#endif

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <memory>
#include <cstdio>

class EventLoop
	{
	private:
		typedef void (*Destructor)(EventLoop* eventloop);
	public:
		EventLoop() noexcept:m_n_windows(0)
			{}

		static std::unique_ptr<EventLoop,Destructor> create()
			{return {instanceCreate(),destroy};}

		virtual void run()=0;

		void windowRegister()
			{++m_n_windows;}

		void windowUnregister()
			{--m_n_windows;}

		bool windowsLeft() const
			{return m_n_windows!=0;	}

	protected:
		virtual ~EventLoop()=default;

	private:
		static EventLoop* instanceCreate();
		static void destroy(EventLoop* eventloop)
			{eventloop->run();}
		size_t m_n_windows;
	};

#endif
