#ifdef __WAND__
target[name[test3] type[application] platform[;GNU/Linux]]
#endif

#include "keyboardview.h"
#include "keyboardlayout.h"
#include "mainloop.h"
#include "guicontainer.h"

#include <cstdio>
#include <cstdlib>

class KeyboardViewEvents:public KeyboardView::EventHandler
	{
	public:
		KeyboardViewEvents(KeyboardLayout& model):r_model(&model)
			,r_key_prev(nullptr)
			{}


		void viewSet(KeyboardView& view)
			{r_view=&view;}

		void mouseMove(uint8_t scancode,keymask_t keymask)
			{
			bool dirty=0;
			auto key=r_model->keyFromScancode(scancode);
			if(r_key_prev!=nullptr && key!=r_key_prev)
				{
				r_key_prev->colorSet({0,0,0,1});
				dirty=1;
				}
			if(key!=nullptr && key!=r_key_prev)
				{
				key->colorSet({0,1,0,1});
				dirty=1;
				}
			if(dirty)
				{r_view->update();}
			r_key_prev=key;
			}

		void mouseDown(uint8_t scancode,keymask_t keymask)
			{
			auto key=r_model->keyFromScancode(scancode);
			if(key!=nullptr)
				{printf("[%s",key->labelGet());fflush(stdout);}
			}

		void mouseUp(uint8_t scancode,keymask_t keymask)
			{
			auto key=r_model->keyFromScancode(scancode);
			if(key!=nullptr)
				{printf("-%s]",key->labelGet());fflush(stdout);}
			}


	private:
		KeyboardView* r_view;
		KeyboardLayout* r_model;
		KeyboardLayout::KeyDescriptor* r_key_prev;
	};

class WindowEvents:public GuiContainer::EventHandler
	{
	public:
		WindowEvents():m_scancode_prev(0){}

		void keyDown(uint8_t scancode)
			{
			if(m_scancode_prev!=scancode)
				{
				auto key=r_model->keyFromScancode(scancode);
				if(key!=nullptr)
					{
					key->colorBorderSet({1,0,0,1});
					r_view->update();
					}

				m_scancode_prev=scancode;
				}
			}

		void keyUp(uint8_t scancode)
			{
			auto key=r_model->keyFromScancode(scancode);
			if(key!=nullptr)
				{
				key->colorBorderSet({0.5,0.5,0.5,1});
				r_view->update();
				}
			m_scancode_prev=0;
			}

		void  viewSet(KeyboardView& view)
			{r_view=&view;}

		void modelSet(KeyboardLayout& model)
			{r_model=&model;}

	private:
		uint8_t m_scancode_prev;
		KeyboardView* r_view;
		KeyboardLayout* r_model;
	};

int main(int argc,char** argv)
	{
	init(argc,argv);
	KeyboardLayout keyboard;

	WindowEvents we;
	we.modelSet(keyboard);
	auto mainwin=GuiContainer::create(we);

	KeyboardViewEvents se(keyboard);
	auto keyboardview=KeyboardView::create(*mainwin,se,keyboard);
	se.viewSet(*keyboardview);
	we.viewSet(*keyboardview);
	run();
	return 0;
	}
