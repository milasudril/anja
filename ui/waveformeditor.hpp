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

		private:
			WaveformView m_waveform;
			Box m_box;
				Box m_filename;
					Label m_filename_label;
					TextEntry m_filename_input;
				Box m_description;
					Label m_description_label;
					TextEntry m_description_input;
				Paned m_details;
					Box m_details_left;
						Box m_color;
							Label m_color_label;
							TextEntry m_color_input;
						Box m_channel;
							Label m_channel_label;
							Listbox m_channel_input;
		};

	}

#endif // ANJA_WAVEFORMEDITOR_HPP
