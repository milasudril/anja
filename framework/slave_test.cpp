#ifdef __WAND__
target[name[slave_test] type[application] platform[;GNU/Linux]]
#endif

#include "widget.h"
#include "button.h"
#include "boxhorizontal.h"
#include "window.h"
#include "eventloop.h"

class Test:public Widget
	{
	public:
		static Test* create(GuiContainer& parent)
			{return new Test(parent);}

		void destroy()
			{
			delete this;
			}

		const GuiHandle& handleNativeGet() const
			{return m_box->handleNativeGet();}

	private:
		BoxHorizontal* m_box;
		Button*  m_killme;

		class ActionHandler:public Button::EventHandler
			{
			public:
				ActionHandler(Test& obj):r_obj(obj)
					{}

				void onActionPerform(Button& source)
					{r_obj.destroy();}

			private:
				Test& r_obj;
			} m_handler;

		Test(GuiContainer& parent):m_handler(*this)
			{
			m_box=BoxHorizontal::create(parent);
			m_box->slaveAssign(*this);
			m_killme=Button::create(*m_box,m_handler,0,"Kill Me");
			}

		~Test()
			{
			m_killme->destroy();
			m_box->slaveRelease();
			m_box->destroy();
			}
	};

int main()
	{
	auto loop=EventLoop::create();
	auto mainwin=Window::create(*loop.get());
	auto test=Test::create(*mainwin);
	return 0;
	}
