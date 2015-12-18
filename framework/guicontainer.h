#ifdef __WAND__
target[name[guicontainer.h] type[include]]
#endif

#ifndef GUICONTAINER_H
#define GUICONTAINER_H

class GuiHandle;

class GuiContainer
	{
	public:
		virtual void destroy()=0;
		virtual void componentAdd(const GuiHandle& component)=0;
		virtual void componentRemove(const GuiHandle& component)=0;

	protected:
		virtual ~GuiContainer()=default;
	};

#endif
