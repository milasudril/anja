#ifdef __WAND__
target[name[waveformdataview.o] type[object]]
#endif

#include "waveformdataview.h"
#include "waveformdata.h"
#include "waveformrangeview.h"
#include "units.h"
#include "framework/boxvertical.h"
#include "framework/boxhorizontal.h"
#include "framework/label.h"
#include "framework/textbox.h"
#include "framework/filenamepicker.h"
#include "framework/slider.h"
#include "framework/messagedisplay.h"
#include "framework/window.h"
#include "framework/colorpicker.h"
#include "framework/optionbox.h"
#include "framework/floatconv.h"

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
		{
		m_picker->destroy();
		m_colordlg->destroy();
		}
	}

void WaveformDataView::ColorEventHandler::onButtonClick(InputEntry& source)
	{
	if(m_colordlg==nullptr)
		{
		m_colordlg=Window::create(source,reinterpret_cast<void**>(&m_colordlg));
		m_colordlg->titleSet("Choose a color");
		m_color=r_view->waveformDataGet().keyColorGet();
		m_picker=ColorPicker::create(*m_colordlg
			,m_color
			,m_color_presets.begin(),m_color_presets.length(),*this);
		}
	}

void WaveformDataView::ColorEventHandler::onTextChanged(InputEntry& source)
	{
	r_view->colorUpdate(source.textGet());
	}

void WaveformDataView::ColorEventHandler::onConfirmed(ColorPicker::Tag x)
	{
	r_view->colorUpdate(m_color);
	}



double WaveformDataView::PlaybackGainHandler::valueGet(Slider& source,const char* text)
	{
	auto v=convert(text);
	r_view->gainSet(dBToAmplitude(v));
	return v;
	}

void WaveformDataView::PlaybackGainHandler::textGet(Slider& source,double value,TextBuffer& buffer)
	{
	r_view->gainSet(dBToAmplitude(value));
	sprintf(buffer.begin(),"%.3f",value);
	}

double WaveformDataView::PlaybackGainHandler::valueMap(Slider& source,double x)
const noexcept
	{
	return 145.0*(x - 1.0) + 6.0*x;
	}

double WaveformDataView::PlaybackGainHandler::valueMapInverse(Slider& source,double y)
const noexcept
	{
	return (y+145.0)/151;
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

void WaveformDataView::CommandHandler::onSet(OptionBox& source
	,unsigned int index)
	{
	r_view->optionSet(index);
	}

void WaveformDataView::CommandHandler::onUnset(OptionBox& source
	,unsigned int index)
	{
	r_view->optionUnset(index);
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
	,m_pbgain_events(*this)
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
		m_box_main->insertModeSet(BoxVertical::INSERTMODE_EXPAND
					|BoxVertical::INSERTMODE_FILL
					|BoxVertical::INSERTMODE_END);
		m_box_details=BoxHorizontal::create(*m_box_main);
			m_box_left=BoxVertical::create(*m_box_details);
				m_color=InputEntry::create(*m_box_left,"Color:","...",m_color_events,0);

				m_playback_gain_box=BoxHorizontal::create(*m_box_left);
					m_playback_gain_box->insertModeSet(BoxHorizontal::INSERTMODE_TOP);
					m_playback_gain_label=Label::create(*m_playback_gain_box,"Playback gain/dB:");
					m_playback_gain_box->insertModeSet(BoxHorizontal::INSERTMODE_END);
					m_playback_gain_input=Slider::create(*m_playback_gain_box,m_pbgain_events,1);

				m_options=OptionBox::create(*m_box_left,m_command_handler
					,"Options:",Waveform::FLAG_NAMES);

			m_box_details->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
					|BoxHorizontal::INSERTMODE_FILL
					|BoxHorizontal::INSERTMODE_END);

			m_trim_input=WaveformRangeView::create(*m_box_details,handler_range);
	}

WaveformDataView::~WaveformDataView()
	{
			m_trim_input->destroy();
				m_options->destroy();
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
	auto& waveform=r_data->waveformGet();
	m_trim_input->waveformSet(waveform);

	m_source->textSet(r_data->filenameGet().begin());
	m_description_textbox->textSet(r_data->descriptionGet().begin());
	WaveformData::ColorString string;
	r_data->keyColorGet(string);
	m_color->textSet(string.begin());

	m_playback_gain_input->valueSet(amplitudeToDb(waveform.gainGet()));

	auto N_options=m_options->nOptionsGet();
	for(uint32_t k=0;k<N_options;++k)
		{
		m_options->stateSet(k,waveform.flagGet(k));
		}
	}

void WaveformDataView::waveformLoad(const char* filename)
	{
	r_handler->onSourceChange(*this,filename);
	}

void WaveformDataView::descriptionUpdate()
	{
	r_handler->onDescriptionChange(*this,m_description_textbox->textGet());
	}

void WaveformDataView::colorUpdate(const ColorRGBA& color_new)
	{
	r_handler->onColorChange(*this,color_new);
	}

void WaveformDataView::colorUpdate(const char* colorstr)
	{
	r_handler->onColorChange(*this,colorstr);
	}

void WaveformDataView::gainSet(float gain)
	{
	r_handler->onGainChange(*this,gain);
	}

void WaveformDataView::optionUnset(uint32_t option)
	{
	r_handler->onOptionUnset(*this,option);
	}

void WaveformDataView::optionSet(uint32_t option)
	{
	r_handler->onOptionSet(*this,option);
	}
