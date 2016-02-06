#ifdef __WAND__
target[name[sessiondataview.o] type[object]]
#endif

#include "sessiondataview.h"
#include "session.h"
#include "framework/guicontainer.h"
#include "framework/scrollwindow.h"
#include "framework/textbox.h"
#include "framework/label.h"
#include "framework/boxhorizontal.h"
#include "framework/boxvertical.h"
#include "framework/optionbox.h"

SessionDataView* SessionDataView::create(GuiContainer& parent,Session& session)
	{return new SessionDataView(parent,session);}

void SessionDataView::destroy()
	{delete this;}

const GuiHandle& SessionDataView::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

SessionDataView::SessionDataView(GuiContainer& parent,Session& session)
	{
	m_box=ScrollWindow::create(parent,ScrollWindow::MODE_VERTICAL);
	m_box->slaveAssign(*this);
	m_vbox=BoxVertical::create(*m_box);

	m_title=BoxHorizontal::create(*m_vbox);
	m_title_label=Label::create(*m_title,"Title:");
	m_title->insertModeSet(BoxHorizontal::INSERTMODE_END
		|BoxHorizontal::INSERTMODE_EXPAND
		|BoxHorizontal::INSERTMODE_FILL);
	m_title_input=Textbox::create(*m_title);

	m_description=BoxHorizontal::create(*m_vbox);
	m_description_label=Label::create(*m_description,"Description:");
	m_description->insertModeSet(BoxHorizontal::INSERTMODE_END
		|BoxHorizontal::INSERTMODE_EXPAND
		|BoxHorizontal::INSERTMODE_FILL);
	m_description_input=Textbox::create(*m_description);

	m_options=OptionBox::create(*m_vbox,"Options:",Session::FLAG_NAMES);

	sessionSet(session);
	}

	SessionDataView::~SessionDataView()
	{
	m_options->destroy();
	m_description_input->destroy();
	m_description_label->destroy();
	m_description->destroy();
	m_title_input->destroy();
	m_title_label->destroy();
	m_title->destroy();
	m_vbox->destroy(),
	m_box->slaveRelease();
	m_box->destroy();
	}

void SessionDataView::update()
	{
	m_title_input->textSet(r_session->titleGet().begin());
	m_description_input->textSet(r_session->descriptionGet().begin());
	}
