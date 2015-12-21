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
			,const char* text_button,unsigned int id)
			{return create(parent,label,text_button,s_default_handler,id);}

		static InputEntry* create(GuiContainer& parent,const char* label
			,const char* text_button,EventHandler& handler,unsigned int id)
			{return new InputEntry(parent,label,text_button,handler,id);}

		const char* textGet() const;
		void textSet(const char* text);
		void destroy();
		unsigned int idGet() const
			{return m_id;}

		const GuiHandle& handleNativeGet() const;


	private:
		InputEntry(GuiContainer& parent,const char* label
			,const char* text_button,EventHandler& handler,unsigned int id);
		~InputEntry()=default;

		class EventHandlerInternal:public BoxHorizontal::EventHandler
			{
			public:
				EventHandlerInternal(InputEntry& object
					,InputEntry::EventHandler& handler);

				static constexpr unsigned int COMMAND_ACTION=1;
				static constexpr unsigned int COMMAND_TEXTCHANGED=2;

				void onCommand(BoxHorizontal& source,unsigned int command_id);

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

		int m_id;

		static EventHandler s_default_handler;
	};

#endif
