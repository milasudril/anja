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
			{return instanceCreate(parent,keyboard,handler);}

		static KeyboardView* create(GuiContainer& parent,const KeyboardLayout& keyboard)
			{return instanceCreate(parent,keyboard,s_null_handler);}

		virtual void destroy()=0;
		virtual void update()=0;
		virtual void eventHandlerSet(EventHandler& eh)=0;
		virtual void keyboardLayoutSet(const KeyboardLayout& layout)=0;

	protected:
		virtual ~KeyboardView()=default;

	private:
		static KeyboardView* instanceCreate(GuiContainer& parent,const KeyboardLayout& keyboard
			,EventHandler& handler);

		static EventHandler s_null_handler;
	};

#endif

