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

		void run();

		void ui_update(UiUpdate update,void* cb)
			{
			gdk_threads_add_idle(update,cb);
			}

	private:
	//	static gboolean update_callback(gpointer data)

		volatile bool m_stop;
	};

UiContext::UiContext()
	{m_impl=new Impl;}

UiContext::~UiContext()
	{delete m_impl;}

void UiContext::exit()
	{m_impl->exit();}

void UiContext::run()
	{m_impl->run();}

void UiContext::ui_update(UiUpdate update,void* cb)
	{m_impl->ui_update(update,cb);}


UiContext::Impl::~Impl()
	{m_impl=nullptr;}

void UiContext::Impl::run()
	{
	m_stop=0;
	while(!m_stop)
		{
		gtk_main_iteration();
		}
	}
