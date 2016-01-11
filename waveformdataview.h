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

class GuiContainer;
class BoxVertical;
class BoxHorizontal;
class Label;
class Slider;
class WaveformData;
class Window;

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
				virtual void onOptionSet(WaveformDataView& source
					,uint32_t option)=0;
				virtual void onOptionUnset(WaveformDataView& source
					,uint32_t option)=0;
			};

		static WaveformDataView* create(GuiContainer& parent
			,EventHandler& handler
			,WaveformRangeView::EventHandler& handler_range);

		void destroy();
		const GuiHandle& handleNativeGet() const;

		void waveformDataSet(WaveformData& wd);

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

		class SourceEventHandler:public InputEntry::EventHandler
			{
			public:
				SourceEventHandler(WaveformDataView& view):r_view(&view)
					{}
				void onButtonClick(InputEntry& source);
				void onTextChanged(InputEntry& source);

			private:
				WaveformDataView* r_view;
			} m_source_events;

		class CommandHandler:public BoxVertical::EventHandler
			,public OptionBox::EventHandler
			{
			public:
				CommandHandler(WaveformDataView& view):r_view(&view)
					{}

				void onCommand(BoxVertical& source,unsigned int command_id);

				void onSet(OptionBox& source,unsigned int option_id);
				void onUnset(OptionBox& source,unsigned int option_id);

			private:
				WaveformDataView* r_view;
			} m_command_handler;

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

		friend class SourceEventHandler;
		friend class CommandHandler;

		void waveformLoad(const char* filename);
		void descriptionUpdate();
		void colorUpdate(const ColorRGBA& color_new);
		void colorUpdate(const char* colorstr);
		void optionSet(uint32_t option);
		void optionUnset(uint32_t option);


		BoxVertical* m_box_main;
			InputEntry* m_source;
			BoxHorizontal* m_description_box;
				Label* m_description_label;
				Textbox* m_description_textbox;
			BoxHorizontal* m_box_details;
				BoxVertical* m_box_left;
					InputEntry* m_color;

					BoxHorizontal* m_playback_gain_box;
						Label* m_playback_gain_label;
						Slider* m_playback_gain_input;

					OptionBox* m_options;

				BoxVertical* m_trim_box;
					Label* m_trim_label;
					WaveformRangeView* m_trim_input;
	};

#endif
