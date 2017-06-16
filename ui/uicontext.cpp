//@	{
//@	 "targets":[{"name":"uicontext.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "uicontext.hpp"
#include "../common/thread.hpp"
#include "../common/readysignal.hpp"
#include "../common/mutex.hpp"
#include "../common/ringbuffer.hpp"
#include <gtk/gtk.h>

using namespace Anja;

static const char* STYLESHEET=R"EOF(
GtkEntry,GtkButton{padding-top:2px;padding-bottom:2px;padding-left:4px}
GtkButton{padding-right:4px}
GtkComboBox > * {padding-top:0px;padding-bottom:0px}
GtkNotebook > *
	{
	background-color:rgba(0,0,0,0.0);
	border:0px solid rgba(0,0,0,0);
	}

GtkNotebook
	{
	background-color:shade(@theme_bg_color,1.2);
	}

GtkNotebook > tab
	{
	padding:2px;border-top:1px rgba(160,160,160,0.5) solid;
	border-left:1px rgba(160,160,160,0.5) solid;
	border-right:1px rgba(160,160,160,0.5) solid;
	border-bottom:0px rgba(160,160,160,0.5) solid;
	border-radius:4px 4px 0px 0px;
	background-color:rgba(0,0,0,0)
	}

GtkNotebook > tab:active
	{
	border-top:2px rgba(160,160,160,1) solid;
	border-left:2px rgba(160,160,160,1) solid;
	border-right:2px rgba(160,160,160,1) solid;
	border-bottom:0px rgba(160,160,160,1) solid;
	padding:3px;
	}

)EOF";

class UiContext::Impl:private UiContext
	{
	public:
		Impl():UiContext(*this),m_stop(0),m_messages(1024),m_dark(0)
			{
			gtk_disable_setlocale();
			gtk_init(NULL,NULL);
			auto provider=gtk_css_provider_get_default();
			gtk_css_provider_load_from_data(provider,STYLESHEET,-1,NULL);
			gtk_style_context_add_provider_for_screen(gdk_screen_get_default()
				,GTK_STYLE_PROVIDER(provider)
				,GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
			}

		~Impl();

		void exit()
			{
			m_stop=1;
			m_ready.set();
			m_processed.set();
			}

		void run(const Vtable& vt,void* cb_obj)
			{
			m_vt=vt;
			r_cb_obj=cb_obj;
			m_stop=0;
			Thread idle_loop(*this,std::integral_constant<int,0>{});
			while(!m_stop)
				{gtk_main_iteration_do(m_vt.idle(cb_obj,*this)==RunStatus::WAIT);}
			}

		template<int id>
		void run()
			{
			while(!m_stop)
				{
				while(m_messages.empty() && !m_stop)
					{m_ready.wait();}
				if(!m_stop)
					{
					m_message_current=m_messages.pop_front();
					g_idle_add([](void* impl)
						{
						auto self=reinterpret_cast<Impl*>(impl);
						auto msg=self->m_message_current;
						self->m_processed.set();
						self->m_vt.process(self->r_cb_obj,*self
							,static_cast<int32_t>(msg>>32llu),msg&(~0xffffffff00000000));
						return G_SOURCE_REMOVE;
						},this);
					m_processed.wait();
					}
				}
			}

		void dark(bool status)
			{
			g_object_set(gtk_settings_get_default(),"gtk-application-prefer-dark-theme"
				,status,NULL);
			m_dark=status;
			}

		bool dark() const noexcept
			{
			return m_dark;
			}

		bool messagePostTry(int32_t id,int32_t param) noexcept
			{
			if(!m_mtx.lockTry())
				{return 0;}
			if(m_messages.full())
				{
				m_mtx.unlock();
				return 0;
				}
			m_messages.push_back((uint64_t(id)<<32llu)|param);
			m_mtx.unlock();
			m_ready.set();
			return 1;
			}

		void messagePost(int32_t id,int32_t param)
			{
			Mutex::LockGuard g(m_mtx);
			while(m_messages.full())
				{
				m_ready.set();
				sched_yield();
				}
			m_messages.push_back((uint64_t(id)<<32llu)|param);
			m_ready.set();
			}

	private:
		volatile bool m_stop;
		RingBuffer<uint64_t,volatile uint32_t> m_messages;
		uint64_t m_message_current;
		Vtable m_vt;
		void* r_cb_obj;
		ReadySignal m_ready;
		ReadySignal m_processed;
		Mutex m_mtx;
		bool m_dark;
	};

UiContext::UiContext()
	{m_impl=new Impl;}

UiContext::~UiContext()
	{delete m_impl;}

void UiContext::exit()
	{m_impl->exit();}

void UiContext::run(const Vtable& vt,void* cb_obj)
	{m_impl->run(vt,cb_obj);}

UiContext& UiContext::dark(bool status)
	{
	m_impl->dark(status);
	return *this;
	}

bool UiContext::dark() const noexcept
	{return m_impl->dark();}

UiContext::Impl::~Impl()
	{m_impl=nullptr;}


bool UiContext::messagePostTry(int32_t id,int32_t param) noexcept
	{
	return m_impl->messagePostTry(id,param);
	}

UiContext& UiContext::messagePost(int32_t id,int32_t param)
	{
	m_impl->messagePost(id,param);
	return *this;
	}

