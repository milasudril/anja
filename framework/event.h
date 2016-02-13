#ifdef __WAND__
target[name[event.h] type[include]]
dependency[event.o]
#endif

#ifndef EVENT_H
#define EVENT_H

#include <memory>

class Event
	{
	public:
		static Event* create();
		virtual void destroy()=0;

		virtual void set()=0;
		virtual void wait()=0;

	protected:
		virtual ~Event()=default;
	};

#endif
