#ifdef __WAND__
target[name[waveformdataview.h] type[include]]
dependency[waveformdataview.o]
#endif

#ifndef WAVEFORMDATAVIEW_H
#define WAVEFORMDATAVIEW_H

#include "framework/widget.h"

#include "waveformrangeview.h"
#include "framework/inputentry.h"
#include "framework/color.h"
#include "framework/colorpicker.h"
#include "framework/optionbox.h"
#include "framework/slider.h"
#include "framework/listbox.h"

class GuiContainer;
class BoxVertical;
class BoxHorizontal;
class Label;
class WaveformData;
class Window;
class ChannelData;
class ScrollWindow;

class WaveformDataView:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onSourceChange(WaveformDataView& source
					,const char* filename_new)=0;
				virtual void onDescriptionChange(WaveformDataView& source
					,const char* description_new)=0;
				virtual void onColorChange(WaveformDataView& source
					,const ColorRGBA& color_new)=0;
				virtual void onColorChange(WaveformDataView& source
					,const char* colorstr)=0;
				virtual void onGainChange(WaveformDataView& source
					,float value)=0;
				virtual void onGainRandomChange(WaveformDataView& source
					,float value)=0;
				virtual void onOptionSet(WaveformDataView& source
					,unsigned int option)=0;
				virtual void onOptionUnset(WaveformDataView& source
					,unsigned int option)=0;
				virtual void onChannelChange(WaveformDataView& source
					,unsigned int channel)=0;
			};

		void doSourceChange(const char* filename_new)
			{r_handler->onSourceChange(*this,filename_new);}

		void doDescriptionChange(const char* description_new)
			{r_handler->onDescriptionChange(*this,description_new);}

		void doColorChange(const ColorRGBA& color_new);

		void doColorChange(const char* colorstr)
			{r_handler->onColorChange(*this,colorstr);}

		void doGainChange(float value)
			{r_handler->onGainChange(*this,value);}

		void doGainRandomChange(float value)
			{r_handler->onGainRandomChange(*this,value);}

		void doOptionSet(unsigned int option)
			{r_handler->onOptionSet(*this,option);}

		void doOptionUnset(unsigned int option)
			{r_handler->onOptionUnset(*this,option);}

		void doChannelChange(unsigned int channel)
			{r_handler->onChannelChange(*this,channel);}



		static WaveformDataView* create(GuiContainer& parent
			,EventHandler& handler
			,WaveformRangeView::EventHandler& handler_range);

		void destroy();
		const GuiHandle& handleNativeGet() const;

		void waveformDataSet(WaveformData& wd);

		void channelSelectorInit(const ChannelData* channels,unsigned int n_ch);

		void channelNameUpdate(const ChannelData& channel,unsigned int id);

		WaveformData& waveformDataGet()
			{return *r_data;}

		const WaveformData& waveformDataGet() const
			{return *r_data;}

		void update();

	private:
		WaveformDataView(GuiContainer& parent
			,EventHandler& handler
			,WaveformRangeView::EventHandler& handler_range);
		~WaveformDataView();

		GuiContainer& r_parent;
		EventHandler* r_handler;
		WaveformData* r_data;

		class EventHandlerInternal:public InputEntry::EventHandler
			,public Textbox::EventHandler
			,public OptionBox::EventHandler
			,public ValueInput::EventHandler
			,public Listbox::EventHandler
			{
			public:
				EventHandlerInternal(WaveformDataView& view);

				void onButtonClick(InputEntry& source);
				void onTextChanged(InputEntry& source);

				void onLeave(Textbox& source);

				void onSet(OptionBox& source,unsigned int option_id);
				void onUnset(OptionBox& source,unsigned int option_id);

				double valueGet(ValueInput& source,const char* text);
				void textGet(ValueInput& source,double value,TextBuffer& buffer);
				double valueMap(ValueInput& source,double x) const noexcept;
				double valueMapInverse(ValueInput& source,double y) const noexcept;

				void onOptionSelect(Listbox& source);

			private:
				WaveformDataView* r_view;
			} m_handler;

		class ColorEventHandler:public InputEntry::EventHandler
			,public ColorPicker::EventHandler
			{
			public:
				ColorEventHandler(WaveformDataView& view);
				~ColorEventHandler();
				void onButtonClick(InputEntry& source);
				void onTextChanged(InputEntry& source);
				void onConfirmed(ColorPicker::Tag x);

			private:
				WaveformDataView* r_view;
				Window* m_colordlg;
				ColorPicker* m_picker;
				ColorRGBA m_color;
				ArraySimple<ColorRGBA> m_color_presets;
			} m_color_events;

		BoxVertical* m_box_main;
			InputEntry* m_source;
			BoxHorizontal* m_description_box;
				Label* m_description_label;
				Textbox* m_description_textbox;
			BoxHorizontal* m_box_details;
				ScrollWindow* m_scroll_left;
					BoxVertical* m_box_left;
						InputEntry* m_color;
						BoxHorizontal* m_playback_channel_box;
							Label* m_playback_channel_label;
							Listbox* m_playback_channel_input;
						Slider* m_playback_gain;
						Slider* m_pbgain_randomize;
						OptionBox* m_options;

				WaveformRangeView* m_trim_input;
	};

#endif
