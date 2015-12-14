#ifdef __WAND__
target[name[guicontainer.h] type[include]]
dependency[guicontainer.o]
#endif

#ifndef GUICONTAINER_H
#define GUICONTAINER_H

#include <cstdint>

class GuiContainer
	{
	public:
		class EventHandler
			{
			public:
				virtual void keyDown(uint8_t scancode)
					{}

				virtual void keyUp(uint8_t scancode)
					{}
			};

		static GuiContainer* create(EventHandler& handler)
			{return new GuiContainer(handler);}

		void destroy()
			{delete this;}

		void objectAdd(void* handle_native);

	private:
		GuiContainer(EventHandler& handler);
		~GuiContainer();
		class Impl;
		Impl* m_impl;
	};

#endif
