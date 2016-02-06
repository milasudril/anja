#ifdef __WAND__
target[name[sessiondataview.o] type[object]]
#endif

#include "sessiondataview.h"
#include "session.h"
#include "framework/guicontainer.h"
#include "framework/scrollwindow.h"
#include "framework/label.h"
#include "framework/boxhorizontal.h"
#include "framework/boxvertical.h"

static constexpr unsigned int TEXTBOX_TITLE=0;
static constexpr unsigned int TEXTBOX_DESCRIPTION=1;

SessionDataView::InputHandler::InputHandler(SessionDataView& view):r_view(&view)
	{}

void SessionDataView::InputHandler::onLeave(Textbox& source)
	{
	switch(source.idGet())
		{
		case TEXTBOX_TITLE:
			r_view->doTitleChange(source.textGet());
			break;
		case TEXTBOX_DESCRIPTION:
			r_view->doDescriptionChange(source.textGet());
			break;
		}
	}

void SessionDataView::InputHandler::onSet(OptionBox& source,unsigned int index)
	{
	r_view->doOptionSet(index);
	}

void SessionDataView::InputHandler::onUnset(OptionBox& source,unsigned int index)
	{
	r_view->doOptionUnset(index);
	}



SessionDataView* SessionDataView::create(GuiContainer& parent,Session& session
	,EventHandler& handler)
	{return new SessionDataView(parent,session,handler);}

void SessionDataView::destroy()
	{delete this;}

const GuiHandle& SessionDataView::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

SessionDataView::SessionDataView(GuiContainer& parent,Session& session
	,EventHandler& handler):r_handler(&handler),m_handler(*this)
	{
	m_box=ScrollWindow::create(parent,ScrollWindow::MODE_VERTICAL);
	m_box->slaveAssign(*this);
	m_vbox=BoxVertical::create(*m_box);

	m_title=BoxHorizontal::create(*m_vbox);
	m_title_label=Label::create(*m_title,"Title:");
	m_title->insertModeSet(BoxHorizontal::INSERTMODE_END
		|BoxHorizontal::INSERTMODE_EXPAND
		|BoxHorizontal::INSERTMODE_FILL);
	m_title_input=Textbox::create(*m_title,m_handler,TEXTBOX_TITLE);

	m_description=BoxHorizontal::create(*m_vbox);
	m_description_label=Label::create(*m_description,"Description:");
	m_description->insertModeSet(BoxHorizontal::INSERTMODE_END
		|BoxHorizontal::INSERTMODE_EXPAND
		|BoxHorizontal::INSERTMODE_FILL);
	m_description_input=Textbox::create(*m_description,m_handler
		,TEXTBOX_DESCRIPTION);

	m_options=OptionBox::create(*m_vbox,m_handler
		,0,"Options:",Session::FLAG_NAMES);

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

	auto N_options=m_options->nOptionsGet();
	for(uint32_t k=0;k<N_options;++k)
		{
		m_options->stateSet(k,r_session->flagGet(k));
		}
	}
