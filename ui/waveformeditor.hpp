//@	{
//@	"targets":[{"name":"waveformeditor.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"waveformeditor.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFORMEDITOR_HPP
#define ANJA_WAVEFORMEDITOR_HPP

#include "box.hpp"
#include "label.hpp"
#include "textentry.hpp"
#include "paned.hpp"
#include "listbox.hpp"
#include "button.hpp"
#include "optionlist.hpp"
#include "slider.hpp"
#include "xyplot.hpp"
#include "../sessiondata/waveformview.hpp"

namespace Anja
	{
	class Session;
	template<class T>
	class ArraySimple;

	class WaveformEditor
		{
		public:
			enum class ButtonId:int
				{FILENAME_BROWSE,FILENAME_RELOAD,COLOR_PICK,CURSORS_SWAP};

			enum class ListboxId:int
				{CHANNEL};

			enum class SliderId:int
				{GAIN,GAIN_RANDOM};

			enum class TextEntryId:int
				{FILENAME,DESCRIPTION,COLOR,GAIN,GAIN_RANDOM};

			enum class OptionListId:int
				{OPTIONS};

			WaveformEditor(Container& cnt,const WaveformView& waveform
				,const ArraySimple<String>& channel_names);

			void clicked(Button& src,ButtonId id);
			void clicked(OptionList<WaveformEditor,OptionListId>& src,Checkbox& opt,int bit);
			void changed(Slider& slider,SliderId id);
			void changed(TextEntry& entry,TextEntryId id);
			void changed(Listbox& lb,ListboxId id);

		private:
			WaveformView m_waveform;
			Box m_box;
				Box m_filename;
					Label m_filename_label;
					TextEntry m_filename_input;
					Button m_filename_browse;
					Button m_filename_reload;
				Box m_description;
					Label m_description_label;
					TextEntry m_description_input;
				Paned m_details;
					Box m_details_left;
						Box m_color;
							Label m_color_label;
							TextEntry m_color_input;
							Button m_color_pick;
						Box m_channel;
							Label m_channel_label;
							Listbox m_channel_input;
						Box m_gain;
							Label m_gain_label;
							Box m_gain_input;
								TextEntry m_gain_input_text;
								Slider m_gain_input_slider;
						Box m_gain_random;
							Label m_gain_random_label;
							Box m_gain_random_input;
								TextEntry m_gain_random_input_text;
								Slider m_gain_random_input_slider;
						Box m_options;
							Label m_options_label;
						OptionList<WaveformEditor,OptionListId> m_options_input;
					Box m_details_right;
					//	XYPlot m_waveform;
						Box m_trim_panel;
							Button m_swap;
		};

	}

#endif // ANJA_WAVEFORMEDITOR_HPP
