#ifdef __WAND__
target[name[waveformdataview.o] type[object]]
#endif

#include "waveformdataview.h"
#include "waveformdata.h"
#include "waveformrangeview.h"
#include "framework/boxvertical.h"
#include "framework/boxhorizontal.h"
#include "framework/label.h"
#include "framework/textbox.h"
#include "framework/filenamepicker.h"
#include "framework/slider.h"
#include "framework/messagedisplay.h"

#include <unistd.h>
#include <cstdio>

void WaveformDataView::SourceEventHandler::onButtonClick(InputEntry& source)
	{
	auto picker=FilenamePicker::create(source,source.textGet()
		,FilenamePicker::MODE_OPEN);
	auto result=picker->filenameGet();
	if(result!=nullptr)
		{
		source.textSet(result);
		}
	}

void WaveformDataView::SourceEventHandler::onTextChanged(InputEntry& source)
	{
	}



WaveformDataView* WaveformDataView::create(GuiContainer& parent
	,WaveformRangeView::EventHandler& handler_range)
	{return new WaveformDataView(parent,handler_range);}

WaveformDataView::WaveformDataView(GuiContainer& parent
	,WaveformRangeView::EventHandler& handler_range):r_parent(parent)
	{
	m_box_main=BoxVertical::create(parent);
	m_box_main->slaveAssign(*this);

		m_source=InputEntry::create(*m_box_main,"Source:","Browse..."
			,m_source_events,0);

		m_description_box=BoxHorizontal::create(*m_box_main);
			m_description_label=Label::create(*m_description_box,"Description:");
			m_description_box->insertModeSet(BoxHorizontal::INSERTMODE_END
				|BoxHorizontal::INSERTMODE_FILL
				|BoxHorizontal::INSERTMODE_EXPAND);
			m_description_textbox=Textbox::create(*m_description_box,1);
		m_box_main->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
					|BoxHorizontal::INSERTMODE_FILL
					|BoxHorizontal::INSERTMODE_END);
		m_box_details=BoxHorizontal::create(*m_box_main);
			m_box_left=BoxVertical::create(*m_box_details);
				m_color=InputEntry::create(*m_box_left,"Color:","...",2);

				m_playback_gain_box=BoxHorizontal::create(*m_box_left);
					m_playback_gain_label=Label::create(*m_playback_gain_box,"Playback gain:");
					m_playback_gain_box->insertModeSet(BoxHorizontal::INSERTMODE_END);
					m_playback_gain_input=Slider::create(*m_playback_gain_box,3);

				m_scancode_box=BoxHorizontal::create(*m_box_left);
					m_scancode_label=Label::create(*m_scancode_box,"Scancode:");
					m_scancode_box->insertModeSet(BoxHorizontal::INSERTMODE_END);
					m_scancode_textbox=Textbox::create(*m_scancode_box,3);

				m_midi_box=BoxHorizontal::create(*m_box_left);
					m_midi_label=Label::create(*m_midi_box,"MIDI note number:");
					m_midi_box->insertModeSet(BoxHorizontal::INSERTMODE_END);
					m_midi_textbox=Textbox::create(*m_midi_box,4);

			m_box_details->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
					|BoxHorizontal::INSERTMODE_FILL
					|BoxHorizontal::INSERTMODE_END);

			m_trim_box=BoxVertical::create(*m_box_details);
				m_trim_label=Label::create(*m_trim_box,"Trim:");
				m_trim_box->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
					|BoxHorizontal::INSERTMODE_FILL
					|BoxHorizontal::INSERTMODE_END);
				m_trim_input=WaveformRangeView::create(*m_trim_box,handler_range);
	}

WaveformDataView::~WaveformDataView()
	{
				m_trim_input->destroy();
				m_trim_label->destroy();
			m_trim_box->destroy();
					m_midi_textbox->destroy();
					m_midi_label->destroy();
				m_midi_box->destroy();
					m_scancode_textbox->destroy();
					m_scancode_label->destroy();
				m_scancode_box->destroy();
					m_playback_gain_input->destroy();
					m_playback_gain_label->destroy();
				m_playback_gain_box->destroy();
				m_color->destroy();
			m_box_left->destroy();
		m_box_details->destroy();
			m_description_textbox->destroy();
			m_description_label->destroy();
		m_description_box->destroy();
		m_source->destroy();
	m_box_main->slaveRelease();
	}

void WaveformDataView::destroy()
	{
	delete this;
	}

const GuiHandle& WaveformDataView::handleNativeGet() const
	{return m_box_main->handleNativeGet();}


void WaveformDataView::waveformDataSet(WaveformData& wd)
	{
	r_data=&wd;
	m_trim_input->waveformRangeSet(wd.waveformRangeGet());
	m_description_textbox->textSet(wd.descriptionGet().begin());
	m_source->textSet(wd.filenameGet().begin());
	}
