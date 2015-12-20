#ifdef __WAND__
target[name[guicontainer.h] type[include]]
#endif

#ifndef GUICONTAINER_H
#define GUICONTAINER_H

#include "widget.h"
#include "keymask.h"
#include <cstdint>

class GuiContainer:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onMouseMove(int x,int y,keymask_t key_mask)
					{}

				virtual void onMouseDown(int x,int y,keymask_t key_mask)
					{}

				virtual void onMouseUp(int x,int y,keymask_t key_mask)
					{}

				virtual void onKeyDown(uint8_t scancode)
					{}

				virtual void onKeyUp(uint8_t scancode)
					{}

				virtual void onCommand(unsigned int command_id)
					{}

				virtual void onDestroy()
					{}
			};

		virtual void componentAdd(const Widget& component)=0;
		virtual void componentRemove(const Widget& component)=0;
		virtual void commandNotify(unsigned int command_id)=0;

	protected:
		virtual ~GuiContainer()=default;
	};

#endif
