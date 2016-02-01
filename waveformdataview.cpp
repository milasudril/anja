#ifdef __WAND__
target[name[waveformdataview.o] type[object]]
#endif

#include "waveformdataview.h"
#include "waveformdata.h"
#include "waveformrangeview.h"
#include "channeldata.h"
#include "colorstring.h"
#include "framework/boxvertical.h"
#include "framework/boxhorizontal.h"
#include "framework/label.h"
#include "framework/textbox.h"
#include "framework/filenamepicker.h"
#include "framework/window.h"
#include "framework/floatconv.h"
#include "framework/scrollwindow.h"

#include <cstring>

WaveformDataView::EventHandlerInternal::EventHandlerInternal(WaveformDataView& view):
	r_view(&view)
	{}

void WaveformDataView::EventHandlerInternal::onButtonClick(InputEntry& source)
	{
	auto picker=FilenamePicker::create(source,source.textGet()
		,FilenamePicker::MODE_OPEN);
	auto result=picker->filenameGet();
	if(result!=nullptr)
		{r_view->doSourceChange(result);}
	}

void WaveformDataView::EventHandlerInternal::onTextChanged(InputEntry& source)
	{
	r_view->doSourceChange(source.textGet());
	}

double WaveformDataView::EventHandlerInternal::valueGet(ValueInput& source,const char* text)
	{
	auto value=convert(text);
	switch(source.idGet())
		{
		case 0:
			r_view->doGainChange(value);
			break;
		case 1:
			r_view->doGainRandomChange(value);
			break;
		}
	return value;
	}

void WaveformDataView::EventHandlerInternal::textGet(ValueInput& source,double value,TextBuffer& buffer)
	{
	switch(source.idGet())
		{
		case 0:
			r_view->doGainChange(value);
			break;
		case 1:
			r_view->doGainRandomChange(value);
			break;
		}
	sprintf(buffer.begin(),"%.3f",value);
	}

double WaveformDataView::EventHandlerInternal::valueMap(ValueInput& source,double x)
const noexcept
	{
	switch(source.idGet())
		{
		case 0:
			return 72.0*(x - 1.0) + 6.0*x;
		case 1:
			return 6.0*x;
		}
	return x;
	}

double WaveformDataView::EventHandlerInternal::valueMapInverse(ValueInput& source,double y)
const noexcept
	{
	switch(source.idGet())
		{
		case 0:
			return (y+72.0)/78;
		case 1:
			return y/6.0;
		}
	return y;
	}

void WaveformDataView::EventHandlerInternal::onLeave(Textbox& source)
	{
	r_view->doDescriptionChange(source.textGet());
	}

void WaveformDataView::EventHandlerInternal::onSet(OptionBox& source
	,unsigned int index)
	{
	r_view->doOptionSet(index);
	}

void WaveformDataView::EventHandlerInternal::onUnset(OptionBox& source
	,unsigned int index)
	{
	r_view->doOptionUnset(index);
	}

void WaveformDataView::EventHandlerInternal::onOptionSelect(Listbox& source)
	{
	r_view->doChannelChange(source.optionSelectedGet());
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
	r_view->doColorChange(source.textGet());
	}

void WaveformDataView::ColorEventHandler::onConfirmed(ColorPicker::Tag x)
	{
	r_view->doColorChange(m_color);
	}



WaveformDataView* WaveformDataView::create(GuiContainer& parent
	,EventHandler& handler
	,WaveformRangeView::EventHandler& handler_range)
	{return new WaveformDataView(parent,handler,handler_range);}

WaveformDataView::WaveformDataView(GuiContainer& parent
	,EventHandler& handler
	,WaveformRangeView::EventHandler& handler_range):r_parent(parent)
	,r_handler(&handler),m_handler(*this),m_color_events(*this)
	{
	m_box_main=BoxVertical::create(parent);
	m_box_main->slaveAssign(*this);

		m_source=InputEntry::create(*m_box_main,m_handler,0,"Source:","Browse...");

		m_description_box=BoxHorizontal::create(*m_box_main);
			m_description_label=Label::create(*m_description_box,"Description:");
			m_description_box->insertModeSet(BoxHorizontal::INSERTMODE_END
				|BoxHorizontal::INSERTMODE_FILL
				|BoxHorizontal::INSERTMODE_EXPAND);
			m_description_textbox=Textbox::create(*m_description_box,m_handler,0);
		m_box_main->insertModeSet(BoxVertical::INSERTMODE_EXPAND
					|BoxVertical::INSERTMODE_FILL
					|BoxVertical::INSERTMODE_END);
		m_box_details=BoxHorizontal::create(*m_box_main);
			m_scroll_left=ScrollWindow::create(*m_box_details,ScrollWindow::MODE_VERTICAL);
				m_box_left=BoxVertical::create(*m_scroll_left);
					m_color=InputEntry::create(*m_box_left,m_color_events,0,"Color:","...");
					m_playback_channel_box=BoxHorizontal::create(*m_box_left);
						m_playback_channel_label=Label::create(*m_playback_channel_box,"Playback channel:");
						m_playback_channel_box->insertModeSet(BoxHorizontal::INSERTMODE_END
							|BoxHorizontal::INSERTMODE_FILL
							|BoxHorizontal::INSERTMODE_EXPAND);
						m_playback_channel_input=Listbox::create(*m_playback_channel_box,m_handler,0);
					m_playback_gain=Slider::create(*m_box_left,m_handler,0
						,"Playback gain/dB:",1);
					m_pbgain_randomize=Slider::create(*m_box_left,m_handler,1
						,"Playback gain random level/dB:",1);
					m_options=OptionBox::create(*m_box_left,m_handler,0
						,"Options:",Waveform::FLAG_NAMES);

			m_box_details->insertModeSet(BoxHorizontal::INSERTMODE_EXPAND
					|BoxHorizontal::INSERTMODE_FILL
					|BoxHorizontal::INSERTMODE_END);

			m_trim_input=WaveformRangeView::create(*m_box_details,handler_range);

			m_scroll_left->heightMinSet(768/2-5*24);
	}

WaveformDataView::~WaveformDataView()
	{
				m_trim_input->destroy();
					m_options->destroy();
					m_pbgain_randomize->destroy();
					m_playback_gain->destroy();
						m_playback_channel_input->destroy();
						m_playback_channel_label->destroy();
					m_playback_channel_box->destroy();
					m_color->destroy();
				m_box_left->destroy();
			m_scroll_left->destroy();
		m_box_details->destroy();
			m_description_textbox->destroy();
			m_description_label->destroy();
		m_description_box->destroy();
		m_source->destroy();
	m_box_main->slaveRelease();
	m_box_main->destroy();
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
	m_color->textSet(ColorString(r_data->keyColorGet()).begin());

	m_playback_gain->valueSet(waveform.gainGet());
	m_pbgain_randomize->valueSet(waveform.gainRandomGet());
	m_playback_channel_input->optionSelect(waveform.channelGet());

	auto N_options=m_options->nOptionsGet();
	for(uint32_t k=0;k<N_options;++k)
		{
		m_options->stateSet(k,waveform.flagGet(k));
		}
	}

void WaveformDataView::channelSelectorInit(const ChannelData* channels,unsigned int n_ch)
	{
	m_playback_channel_input->optionsClear();
	while(n_ch!=0)
		{
		m_playback_channel_input->optionAppend(channels->labelGet().begin());
		++channels;
		--n_ch;
		}
	}

void WaveformDataView::channelNameUpdate(const ChannelData& channel,unsigned int id)
	{
	m_playback_channel_input->optionReplace(id,channel.labelGet().begin());
	}



void WaveformDataView::doColorChange(const ColorRGBA& color_new)
	{
	r_handler->onColorChange(*this,color_new);
	m_color->textSet(ColorString(r_data->keyColorGet()).begin());
	}
