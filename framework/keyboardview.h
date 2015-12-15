#ifdef __WAND__
target[name[keyboardview.h] type[include]]
dependency[keyboardview.o]
#endif

#ifndef KEYBOARDVIEW_H
#define KEYBOARDVIEW_H

#include "keymask.h"
#include <cstdint>

class GuiContainer;
class KeyboardLayout;

class KeyboardView
	{
	public:
		class EventHandler
			{
			public:
				virtual void mouseMove(uint8_t scancode,keymask_t key_mask)
					{}

				virtual void mouseDown(uint8_t scancode,keymask_t key_mask)
					{}

				virtual void mouseUp(uint8_t scancode,keymask_t key_mask)
					{}

				virtual void keyDown(uint8_t scancode)
					{}

				virtual void keyUp(uint8_t scancode)
					{}
			};

		static KeyboardView* create(GuiContainer& parent
			,const KeyboardLayout& keyboard,EventHandler& handler)
			{return new KeyboardView(parent,keyboard,handler);}

		static KeyboardView* create(GuiContainer& parent,const KeyboardLayout& keyboard)
			{
			return new KeyboardView(parent,keyboard,s_null_handler);
			}

		void destroy()
			{delete this;}

		void update();

	private:
		KeyboardView(GuiContainer& parent,const KeyboardLayout& keyboard
			,EventHandler& handler);

		~KeyboardView();

		class Impl;
		Impl* m_impl;

		static EventHandler s_null_handler;
	};

#endif

