#ifdef __WAND__
target[name[sessionview.o] type[object]]
#endif

#include "sessionview.h"
#include "session.h"
#include "waveformdataview.h"
#include "audioconnection.h"
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

SessionView::SessionView(GuiContainer& parent,Session& session):
	m_keyboardevents(*this),m_waveformevents(*this),r_key_current(nullptr)
	{
	m_box=BoxVertical::create(parent);
	m_box->slaveAssign(*this);
	m_box->insertModeSet(BoxVertical::INSERTMODE_EXPAND|BoxVertical::INSERTMODE_FILL);
	m_keyboard=KeyboardView::create(*m_box,session.keyboardLayoutGet(),m_keyboardevents);
	m_box->insertModeSet(BoxVertical::INSERTMODE_END|BoxVertical::INSERTMODE_EXPAND|BoxVertical::INSERTMODE_FILL);
	m_dataview=WaveformDataView::create(*m_box,m_waveformevents,m_trimmer);

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
	slotDisplayFromScancode(41);
	m_keyboardevents.audioConnectionSet(session.audioConnectionGet());
	}

void SessionView::slotDisplayFromScancode(uint8_t scancode)
	{
	if(scancode==0)
		{return;}

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

void SessionView::keyCurrentLabelSet(const char* label)
	{
	r_key_current->labelSet(label);
	m_keyboard->update();
	}

void SessionView::keyCurrentColorSet(const ColorRGBA& color_new)
	{
	r_key_current->colorSet(color_new);
	m_keyboard->update();
	}



void SessionView::KeyboardEventHandler::onKeyDown(KeyboardView& source,uint8_t scancode)
	{
	if(r_connection!=nullptr)
		{r_connection->eventPost(scancode,0);}
	}


void SessionView::WaveformDataEventHandler::onSourceChange(WaveformDataView& source
	,const char* filename_new)
	{
	auto& wd=source.waveformDataGet();
	wd.fileLoad(filename_new);
	source.update();
	}

void SessionView::WaveformDataEventHandler::onDescriptionChange(WaveformDataView& source
	,const char* description_new)
	{
	auto& wd=source.waveformDataGet();
	wd.descriptionSet(description_new);
	source.update();
	r_view->keyCurrentLabelSet(wd.keyLabelGet().begin());
	}

void SessionView::WaveformDataEventHandler::onColorChange(WaveformDataView& source
	,const ColorRGBA& color_new)
	{
	auto& wd=source.waveformDataGet();
	wd.keyColorSet(color_new);
	source.update();
	r_view->keyCurrentColorSet(color_new);
	}

void SessionView::WaveformDataEventHandler::onColorChange(WaveformDataView& source
	,const char* colorstr)
	{
	auto& wd=source.waveformDataGet();
	wd.keyColorSet(colorstr);
	source.update();
	r_view->keyCurrentColorSet(wd.keyColorGet());
	}
