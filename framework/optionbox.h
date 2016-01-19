#ifdef __WAND__
target[name[optionbox.h] type[include]]
dependency[optionbox.o]
#endif

#ifndef OPTIONBOX_H
#define OPTIONBOX_H

#include "widget.h"
#include "checkbox.h"
#include "arraydynamicshort.h"

class BoxVertical;
class Label;

class OptionBox:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onSet(OptionBox& source,unsigned int option_id)
					{}

				virtual void onUnset(OptionBox& source,unsigned int option_id)
					{}
			};

		static OptionBox* create(GuiContainer& parent,const char* title
			,const char* const* options)
			{return create(parent,s_default_handler,0,title,options);}

		static OptionBox* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id,const char* title,const char* const* options)
			{return new OptionBox(parent,handler,id,title,options);}

		unsigned int nOptionsGet() const
			{return m_options.length();}

		bool stateGet(unsigned int option) const;

		void stateSet(unsigned int option,bool state_new);

		void destroy()
			{delete this;}

		const GuiHandle& handleNativeGet() const;

		void doSet(unsigned int id)
			{r_handler->onSet(*this,id);}

		void doUnset(unsigned int id)
			{r_handler->onUnset(*this,id);}

		unsigned int idGet() const
			{return m_id;}

	private:
		EventHandler* r_handler;

		class EventHandlerCheckbox:public Checkbox::EventHandler
			{
			public:
				EventHandlerCheckbox(OptionBox& box);

				void onSet(Checkbox& source);
				void onUnset(Checkbox& source);

			private:
				OptionBox& r_box;
			} m_handler_cbox;

		BoxVertical* m_box;
		Label* m_label;
		ArrayDynamicShort<Checkbox*> m_options;
		unsigned int m_id;

		OptionBox(GuiContainer& parent,EventHandler& handler,unsigned int id
			,const char* title,const char* const* options);
		~OptionBox();

		static EventHandler s_default_handler;
	};

#endif
