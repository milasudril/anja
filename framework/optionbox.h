#ifdef __WAND__
target[name[optionbox.h] type[include]]
dependency[optionbox.o]
#endif

#ifndef OPTIONBOX_H
#define OPTIONBOX_H

#include "widget.h"
#include "boxvertical.h"
#include "arraydynamicshort.h"

class Checkbox;
class Label;

class OptionBox:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onOptionEnable(OptionBox& source,unsigned int option_id)
					{}

				virtual void onOptionDisable(OptionBox& source,unsigned int option_id)
					{}
			};

		static OptionBox* create(GuiContainer& parent,EventHandler& handler
			,const char* title
			,const char* const* options)
			{return new OptionBox(parent,handler,title,options);}

		static OptionBox* create(GuiContainer& parent,const char* title,const char* const* options)
			{return new OptionBox(parent,s_default_handler,title,options);}

		unsigned int nOptionsGet() const
			{return m_options.length();}

		bool stateGet(unsigned int option) const;

		void stateSet(unsigned int option,bool state_new);

		void destroy()
			{delete this;}

		const GuiHandle& handleNativeGet() const;

	private:
		class EventHandlerCheckbox:public BoxVertical::EventHandler
			{
			public:
				EventHandlerCheckbox(OptionBox& box,OptionBox::EventHandler handler):
					r_handler(handler),r_box(box)
					{}

				void onCommand(BoxVertical& source,unsigned int command_id);

			private:
				OptionBox::EventHandler& r_handler;
				OptionBox& r_box;
			} m_handler_cbox;

		BoxVertical* m_box;
		Label* m_label;
		ArrayDynamicShort<Checkbox*> m_options;

		OptionBox(GuiContainer& parent,EventHandler& handler
			,const char* title
			,const char* const* options);
		~OptionBox();

		static EventHandler s_default_handler;
	};

#endif
