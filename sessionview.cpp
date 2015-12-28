#ifdef __WAND__
target[name[sessionview.o] type[object]]
#endif

#include "sessionview.h"
#include "session.h"
#include "waveformdataview.h"
#include "framework/boxvertical.h"
#include "framework/keyboardview.h"

SessionView* SessionView::create(GuiContainer& parent,Session& session)
	{return new SessionView(parent,session);}

void SessionView::destroy()
	{delete this;}

const GuiHandle& SessionView::handleNativeGet() const
	{
	return m_box->handleNativeGet();
	}

SessionView::SessionView(GuiContainer& parent,Session& session):m_keyboardevents(*this)
	,r_key_current(nullptr)
	{
	m_box=BoxVertical::create(parent);
	m_box->slaveAssign(*this);
	m_box->insertModeSet(BoxVertical::INSERTMODE_EXPAND|BoxVertical::INSERTMODE_FILL);
	m_keyboard=KeyboardView::create(*m_box,session.keyboardLayoutGet(),m_keyboardevents);
	m_box->insertModeSet(BoxVertical::INSERTMODE_END);
	m_dataview=WaveformDataView::create(*m_box,m_trimmer);

	sessionSet(session);
	}

SessionView::~SessionView()
	{
	m_dataview->destroy();
	m_keyboard->destroy();
	m_box->slaveRelease();
	}

void SessionView::sessionSet(Session& session)
	{
	r_session=&session;
	auto& keyboard=r_session->keyboardLayoutGet();

	auto scancode_ptr=keyboard.typingAreaScancodesBegin();
	auto scancode_ptr_end=keyboard.typingAreaScancodesEnd();
	while(scancode_ptr!=scancode_ptr_end)
		{
		auto scancode=*scancode_ptr;
		if(scancode!=0)
			{
			auto key=keyboard.keyFromScancode(scancode);
			auto& wd=session.waveformDataFromScancode(scancode);
			auto& key_label_new=wd.keyLabelGet();

			if(key_label_new.length()!=0)
				{key->labelSet(key_label_new.begin());}

			key->colorSet(wd.keyColorGet());
			}
		++scancode_ptr;
		}
	m_keyboard->update();
	}

void SessionView::slotDisplayFromScancode(uint8_t scancode)
	{
	auto key_new=r_session->keyboardLayoutGet().keyFromScancode(scancode);
	if(key_new==nullptr)
		{return;}

	if(r_key_current!=nullptr)
		{r_key_current->colorBorderSet({0.5f,0.5f,0.5f,1.0f});}

	key_new->colorBorderSet(COLORS[ColorID::GREEN]);
	m_keyboard->update();
	m_dataview->waveformDataSet(r_session->waveformDataFromScancode(scancode));
	r_key_current=key_new;
	}
