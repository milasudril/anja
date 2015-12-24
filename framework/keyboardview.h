#ifdef __WAND__
target[name[keyboardview.h] type[include]]
dependency[keyboardview.o]
#endif

#ifndef KEYBOARDVIEW_H
#define KEYBOARDVIEW_H

#include "widget.h"
#include "keymask.h"
#include <cstdint>

class GuiContainer;
class KeyboardLayout;

class KeyboardView:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onMouseMove(KeyboardView& source,uint8_t scancode
					,keymask_t key_mask)
					{}

				virtual void onMouseDown(KeyboardView& source,uint8_t scancode
					,keymask_t key_mask)
					{}

				virtual void onMouseUp(KeyboardView& source,uint8_t scancode
					,keymask_t key_mask)
					{}

				virtual void onKeyDown(KeyboardView& source,uint8_t scancode)
					{}

				virtual void onKeyUp(KeyboardView& source,uint8_t scancode)
					{}
			};

		static KeyboardView* create(GuiContainer& parent
			,const KeyboardLayout& keyboard,EventHandler& handler)
			{return instanceCreate(parent,keyboard,handler);}

		static KeyboardView* create(GuiContainer& parent,const KeyboardLayout& keyboard)
			{return instanceCreate(parent,keyboard,s_default_handler);}

		virtual void destroy()=0;
		virtual void update()=0;
		virtual void eventHandlerSet(EventHandler& eh)=0;
		virtual void keyboardLayoutSet(const KeyboardLayout& layout)=0;
		void keyboardLayoutSet(KeyboardLayout&& layout)=delete;

	protected:
		virtual ~KeyboardView()=default;

	private:
		static KeyboardView* instanceCreate(GuiContainer& parent,const KeyboardLayout& keyboard
			,EventHandler& handler);

		static EventHandler s_default_handler;
	};

#endif

