#ifdef __WAND__
target[name[guihandle.h] type[include]]
#endif

#ifndef GUIHANDLE_H
#define GUIHANDLE_H

#include <cstddef>

template<class SystemType>
class GuiHandleCommon
	{
	public:
		GuiHandleCommon()=default;

		constexpr GuiHandleCommon(SystemType handle):m_handle(handle)
			{}

		operator SystemType() const
			{return m_handle;}

	protected:
		SystemType m_handle;
	};

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class GuiHandle:public GuiHandleCommon<HWND>
	{
	public:
		GuiHandle()=default;

		constexpr GuiHandle(HWND handle):GuiHandleCommon(handle)
			{}
	};

#else
#include <gtk/gtk.h>
class GuiHandle:public GuiHandleCommon<GtkWidget*>
	{
	public:
		GuiHandle()=default;

		constexpr GuiHandle(GtkWidget* handle):GuiHandleCommon(handle)
			{}
	};
#endif
#endif
