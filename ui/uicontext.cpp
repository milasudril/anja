//@	{
//@	 "targets":[{"name":"uicontext.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "uicontext.hpp"
#include <gtk/gtk.h>

using namespace Anja;

class UiContext::Impl:public UiContext
	{
	public:
		Impl():UiContext(*this),m_stop(0)
			{gtk_init(NULL,NULL);}

		~Impl();

		void exit()
			{m_stop=1;}

		void run(IdleCallbackImpl cb,void* cb_obj);

	private:
		volatile bool m_stop;
	};

UiContext::UiContext()
	{m_impl=new Impl;}

UiContext::~UiContext()
	{delete m_impl;}

void UiContext::exit()
	{m_impl->exit();}

void UiContext::run(IdleCallbackImpl cb,void* cb_obj)
	{m_impl->run(cb,cb_obj);}



UiContext::Impl::~Impl()
	{m_impl=nullptr;}

void UiContext::Impl::run(IdleCallbackImpl cb,void* cb_obj)
	{
	m_stop=0;
	bool wait=0;
	while(!m_stop)
		{
		g_main_context_iteration(NULL,wait);
		wait=cb(cb_obj,*this)==RunStatus::WAIT;
		}
	}
