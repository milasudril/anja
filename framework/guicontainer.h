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
		template<class ContainerType>
		class EventHandler
			{
			public:
				virtual void onMouseMove(ContainerType& source,int x,int y
					,keymask_t key_mask)
					{}

				virtual void onMouseDown(ContainerType& source,int x,int y
					,keymask_t key_mask)
					{}

				virtual void onMouseUp(ContainerType& source,int x,int y
					,keymask_t key_mask)
					{}

				virtual void onKeyDown(ContainerType& source,uint8_t scancode)
					{}

				virtual void onKeyUp(ContainerType& source,uint8_t scancode)
					{}
			};

		virtual void componentAdd(Widget& component)=0;
		virtual void componentRemove(Widget& component)=0;

		virtual void slaveAssign(Widget& component)=0;
		virtual void slaveRelease()=0;
		virtual void fullscreenSet(bool state)=0;

		virtual void titleSet(const char* title){}
	};

#endif
