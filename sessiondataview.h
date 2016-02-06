#ifdef __WAND__
target[name[sessiondataview.h] type[include]]
dependency[sessiondataview.o]
#endif

#ifndef SESSIONDATAVIEW_H
#define SESSIONDATAVIEW_H

#include "framework/widget.h"
#include "framework/optionbox.h"
#include "framework/textbox.h"

class GuiContainer;
class ScrollWindow;
class BoxVertical;
class BoxHorizontal;
class Label;
class Session;

class SessionDataView:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onTitleChange(SessionDataView& source,const char* title)=0;
				virtual void onDescriptionChange(SessionDataView& source,const char* description)=0;
				virtual void onOptionSet(SessionDataView& source,unsigned int options)=0;
				virtual void onOptionUnset(SessionDataView& source,unsigned int options)=0;
			};

		static SessionDataView* create(GuiContainer& parent,Session& session
			,EventHandler& handler);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void sessionSet(Session& session)
			{
			r_session=&session;
			update();
			}

		void update();

		void doTitleChange(const char* title)
			{r_handler->onTitleChange(*this,title);}

		void doDescriptionChange(const char* description)
			{r_handler->onDescriptionChange(*this,description);}

		void doOptionSet(unsigned int options)
			{r_handler->onOptionSet(*this,options);}

		void doOptionUnset(unsigned int options)
			{r_handler->onOptionUnset(*this,options);}

		Session& sessionGet() noexcept
			{
			return *r_session;
			}


	private:
		SessionDataView(GuiContainer& parent,Session& session
			,EventHandler& handler);
		~SessionDataView();

		EventHandler* r_handler;
		Session* r_session;

		ScrollWindow* m_box;
		BoxVertical* m_vbox;

		BoxHorizontal* m_title;
			Label* m_title_label;
			Textbox* m_title_input;

		BoxHorizontal* m_description;
			Label* m_description_label;
			Textbox* m_description_input;

		OptionBox* m_options;

		class InputHandler:public Textbox::EventHandler
			,public OptionBox::EventHandler
			{
			public:
				InputHandler(SessionDataView& view);
				void onLeave(Textbox& source);
				void onSet(OptionBox& source,unsigned int option_id);
				void onUnset(OptionBox& source,unsigned int option_id);

			private:
				SessionDataView* r_view;
			} m_handler;
	};

#endif
