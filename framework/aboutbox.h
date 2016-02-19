#ifdef __WAND__
target[name[aboutbox.h] type[include]]
dependency[aboutbox.o]
#endif

#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <memory>

class ProgramInfo;
class Widget;

class AboutBox
	{
	private:
		typedef void (*Destructor)(AboutBox* aboutbox);

	public:
		static std::unique_ptr<AboutBox,Destructor> create(
			  const Widget& owner,const ProgramInfo& info)
			{return {instanceCreate(owner,info),destroy};}

	protected:
		virtual ~AboutBox()=default;

	private:
		static AboutBox* instanceCreate(const Widget& owner
			,const ProgramInfo& info);

		static void destroy(AboutBox* picker);
	};

#endif
