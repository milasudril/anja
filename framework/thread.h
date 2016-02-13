#ifdef __WAND__
target[name[thread.h] type[include]]
dependency[thread.o]
#endif

#ifndef THREAD_H
#define THREAD_H

class Thread
	{
	public:
		class Task
			{
			public:
				virtual unsigned int run()=0;
				virtual void onCompleted(Thread& source) noexcept
					{}
			};

		static Thread* create(Task& task);
		virtual void destroy() noexcept=0;
		virtual unsigned int exitStatusGet() const noexcept=0;

	protected:
		virtual ~Thread()=default;
	};

#endif
