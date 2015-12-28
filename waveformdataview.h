#ifdef __WAND__
target[name[waveformdataview.h] type[include]]
dependency[waveformdataview.o]
#endif

#ifndef WAVEFORMDATAVIEW_H
#define WAVEFORMDATAVIEW_H

#include "framework/widget.h"
#include "framework/inputentry.h"
#include "waveformrangeview.h"

class GuiContainer;
class BoxVertical;
class BoxHorizontal;
class Label;
class Textbox;
class Slider;
class WaveformData;

class WaveformDataView:public Widget
	{
	public:
		static WaveformDataView* create(GuiContainer& parent
			,WaveformRangeView::EventHandler& handler_range);
		void destroy();
		const GuiHandle& handleNativeGet() const;
		void waveformDataSet(WaveformData& wd);

	private:
		WaveformDataView(GuiContainer& parent
			,WaveformRangeView::EventHandler& handler_range);
		~WaveformDataView();

		GuiContainer& r_parent;
		WaveformData* r_data;

		class SourceEventHandler:public InputEntry::EventHandler
			{
			public:
				void onButtonClick(InputEntry& source);
				void onTextChanged(InputEntry& source);
			} m_source_events;


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

					BoxHorizontal* m_scancode_box;
						Label* m_scancode_label;
						Textbox* m_scancode_textbox;

					BoxHorizontal* m_midi_box;
						Label* m_midi_label;
						Textbox* m_midi_textbox;
				BoxVertical* m_trim_box;
					Label* m_trim_label;
					WaveformRangeView* m_trim_input;
	};

#endif
