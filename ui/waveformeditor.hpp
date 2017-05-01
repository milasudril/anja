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
			WaveformEditor(Container& cnt,const WaveformView& waveform
				,const ArraySimple<String>& channel_names);

			void clicked(Button& src);

			void clicked(OptionList<WaveformEditor>& src,Checkbox& opt);

			void changed(Slider& slider);

			void changed(TextEntry& entry);

			void changed(Listbox& lb);

		private:
			WaveformView m_waveform;
			Box m_box;
				Box m_filename;
					Label m_filename_label;
					TextEntry m_filename_input;
					Button m_filename_browse;
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
						OptionList<WaveformEditor> m_options_input;
		};

	}

#endif // ANJA_WAVEFORMEDITOR_HPP
