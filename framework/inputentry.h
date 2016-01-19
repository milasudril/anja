#ifdef __WAND__
target[name[inputentry.h] type[include]]
dependency[inputentry.o]
#endif

#ifndef INPUTENTRY_H
#define INPUTENTRY_H

#include "widget.h"
#include "boxhorizontal.h"
#include "textbox.h"
#include "button.h"

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
			{return create(parent,s_default_handler,0,label,text_button);}

		static InputEntry* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id,const char* label,const char* text_button)
			{return new InputEntry(parent,handler,id,label,text_button);}

		const char* textGet() const;
		void textSet(const char* text);
		void destroy()
			{delete this;}
		unsigned int idGet() const
			{return m_id;}

		const GuiHandle& handleNativeGet() const;

		void doButtonClick()
			{r_handler->onButtonClick(*this);}

		void doTextChanged()
			{r_handler->onTextChanged(*this);}

	private:
		InputEntry(GuiContainer& parent,EventHandler& handler
			,unsigned int id,const char* label,const char* text_button);
		~InputEntry();

		class EventHandlerInternal:public Textbox::EventHandler
			,public Button::EventHandler
			{
			public:
				EventHandlerInternal(InputEntry& object);

				void onActionPerform(Button& source);
				void onLeave(Textbox& source);

			private:
				InputEntry& r_object;
			};

		EventHandler* r_handler;
		EventHandlerInternal m_handler;
		BoxHorizontal* m_box;
		Label* m_label;
		Textbox* m_textbox;
		Button* m_button;

		int m_id;

		static EventHandler s_default_handler;
	};

#endif
