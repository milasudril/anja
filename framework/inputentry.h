#ifdef __WAND__
target[name[inputentry.h] type[include]]
dependency[inputentry.o]
#endif

#ifndef INPUTENTRY_H
#define INPUTENTRY_H

#include "widget.h"
#include "boxhorizontal.h"

class Label;
class Textbox;
class Button;

class InputEntry:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onButtonClick(InputEntry& source)
					{}
				virtual void onTextChanged(InputEntry& source)
					{}
			};

		static InputEntry* create(GuiContainer& parent,const char* label
			,const char* text_button)
			{return create(parent,label,text_button,s_default_handler);}

		static InputEntry* create(GuiContainer& parent,const char* label
			,const char* text_button
			,EventHandler& handler)
			{return new InputEntry(parent,label,text_button,handler);}

		const char* textGet() const;
		void textSet(const char* text);
		void destroy();

		const GuiHandle& handleNativeGet() const;


	private:
		InputEntry(GuiContainer& parent,const char* label
			,const char* text_button,EventHandler& handler);
		~InputEntry()=default;

		class EventHandlerInternal:public BoxHorizontal::EventHandler
			{
			public:
				EventHandlerInternal(InputEntry& object
					,InputEntry::EventHandler& handler);

				static constexpr unsigned int COMMAND_ACTION=1;
				static constexpr unsigned int COMMAND_TEXTCHANGED=2;

				void onCommand(BoxHorizontal& source,unsigned int command_id);
				void onDestroy(BoxHorizontal& source);

			private:
				InputEntry::EventHandler& r_handler;
				InputEntry& r_object;
			};

		GuiContainer& r_parent;
		EventHandlerInternal m_handler;
		BoxHorizontal* m_box;
		Label* m_label;
		Textbox* m_textbox;
		Button* m_button;

		static EventHandler s_default_handler;
	};

#endif
