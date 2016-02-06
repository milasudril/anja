#ifdef __WAND__
target[name[sessiondataview.h] type[include]]
dependency[sessiondataview.o]
#endif

#ifndef SESSIONDATAVIEW_H
#define SESSIONDATAVIEW_H

#include "framework/widget.h"

class GuiContainer;
class ScrollWindow;
class BoxVertical;
class BoxHorizontal;
class Label;
class Textbox;
class OptionBox;
class Session;

class SessionDataView:public Widget
	{
	public:
		static SessionDataView* create(GuiContainer& parent,Session& session);
		void destroy();

		const GuiHandle& handleNativeGet() const;

		void sessionSet(Session& session)
			{
			r_session=&session;
			update();
			}

		void update();

	private:
		SessionDataView(GuiContainer& parent,Session& session);
		~SessionDataView();

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

	};

#endif
