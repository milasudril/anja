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
#include "framework/window.h"
#include "framework/colorpicker.h"

#include <cstring>



void WaveformDataView::SourceEventHandler::onButtonClick(InputEntry& source)
	{
	auto picker=FilenamePicker::create(source,source.textGet()
		,FilenamePicker::MODE_OPEN);
	auto result=picker->filenameGet();
	if(result!=nullptr)
		{r_view->waveformLoad(result);}
	}

void WaveformDataView::SourceEventHandler::onTextChanged(InputEntry& source)
	{
	r_view->waveformLoad(source.textGet());
	}



WaveformDataView::ColorEventHandler::ColorEventHandler(WaveformDataView& view):
	r_view(&view),m_colordlg(nullptr),m_color_presets(64)
	{
	memcpy(m_color_presets.begin(),COLORS
		,std::min(int(ColorID::COLOR_END),64)*sizeof(ColorRGBA));
	}

WaveformDataView::ColorEventHandler::~ColorEventHandler()
	{
	if(m_colordlg!=nullptr)
		{m_colordlg->destroy();}
	}

void WaveformDataView::ColorEventHandler::onButtonClick(InputEntry& source)
	{
	if(m_colordlg==nullptr)
		{
		m_colordlg=Window::create(source,reinterpret_cast<void**>(&m_colordlg));
		m_colordlg->titleSet("Choose a color");
		m_picker=ColorPicker::create(*m_colordlg
			,r_view->waveformDataGet().keyColorGet()
			,m_color_presets.begin(),m_color_presets.length());
		}
	}

void WaveformDataView::ColorEventHandler::onTextChanged(InputEntry& source)
	{
	}



static constexpr unsigned int DESCRIPTION=10;

void WaveformDataView::CommandHandler::onCommand(BoxVertical& source
	,unsigned int command_id)
	{
	switch(command_id)
		{
		case DESCRIPTION:
			r_view->descriptionUpdate();
			break;
		default:
			break;
		}
	}



WaveformDataView* WaveformDataView::create(GuiContainer& parent
	,EventHandler& handler
	,WaveformRangeView::EventHandler& handler_range)
	{return new WaveformDataView(parent,handler,handler_range);}

WaveformDataView::WaveformDataView(GuiContainer& parent
	,EventHandler& handler
	,WaveformRangeView::EventHandler& handler_range):r_parent(parent)
	,r_handler(&handler),m_source_events(*this),m_command_handler(*this)
	,m_color_events(*this)
	{
	m_box_main=BoxVertical::create(parent,&m_command_handler);
	m_box_main->slaveAssign(*this);

		m_source=InputEntry::create(*m_box_main,"Source:","Browse..."
			,m_source_events,0);

		m_description_box=BoxHorizontal::create(*m_box_main);
			m_description_label=Label::create(*m_description_box,"Description:");
			m_description_box->insertModeSet(BoxHorizontal::INSERTMODE_END
				|BoxHorizontal::INSERTMODE_FILL
				|BoxHorizontal::INSERTMODE_EXPAND);
			m_description_textbox=Textbox::create(*m_description_box,DESCRIPTION);
		m_box_main->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
					|BoxHorizontal::INSERTMODE_FILL
					|BoxHorizontal::INSERTMODE_END);
		m_box_details=BoxHorizontal::create(*m_box_main);
			m_box_left=BoxVertical::create(*m_box_details);
				m_color=InputEntry::create(*m_box_left,"Color:","...",m_color_events,0);

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
	update();
	}

void WaveformDataView::update()
	{
	m_trim_input->waveformRangeSet(r_data->waveformRangeGet());
	m_description_textbox->textSet(r_data->descriptionGet().begin());
	m_source->textSet(r_data->filenameGet().begin());
	}

void WaveformDataView::waveformLoad(const char* filename)
	{
	r_handler->onSourceChange(*this,filename);
	}

void WaveformDataView::descriptionUpdate()
	{
	r_handler->onDescriptionChange(*this,m_description_textbox->textGet());
	}
