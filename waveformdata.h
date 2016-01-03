#ifdef __WAND__
target[name[waveformdata.h] type[include]]
dependency[waveformdata.o]
#endif

#ifndef WAVEFORMDATA_H
#define WAVEFORMDATA_H

#include "waveform.h"

#include "framework/color.h"
#include "framework/array_fixed.h"
#include "framework/keyboardlayout.h"
#include <string>

class SessionFileRecord;

class WaveformData
	{
	public:
		WaveformData();

		WaveformData(const SessionFileRecord& record
			,const ArrayDynamicShort<char>& load_path
			,Waveform& storage
			,KeyboardLayout::KeyDescriptor& key);

		void waveformSet(Waveform& waveform_new)
			{
			r_waveform=&waveform_new;
			}

		Waveform& waveformGet()
			{
			return *r_waveform;
			}

		const ArrayDynamicShort<char>& filenameGet() const
			{return m_filename;}

		void fileLoad(const ArrayDynamicShort<char>& filename
			,const ArrayDynamicShort<char>& load_path);

		void fileLoad(const char* filename);

		void keySet(KeyboardLayout::KeyDescriptor* key)
			{r_key=key;}



		const ArrayDynamicShort<char>& descriptionGet() const
			{return m_description;}

		void descriptionSet(const ArrayDynamicShort<char>& description);

		void descriptionSet(const char* description);

		const ArrayDynamicShort<char>& keyLabelGet() const
			{return m_key_label;}



		const ColorRGBA& keyColorGet() const
			{return m_color;}

		typedef ArrayFixed<char,64> ColorString;
		void keyColorGet(ColorString& buffer);

		void keyColorSet(const ColorRGBA& color_new);

		void keyColorSet(const char* colorstring);



	private:
		ArrayDynamicShort<char> m_filename;
		ArrayDynamicShort<char> m_description;
		ArrayDynamicShort<char> m_key_label;
		ColorRGBA m_color;
		KeyboardLayout::KeyDescriptor* r_key;
		Waveform* r_waveform;
		uint8_t m_midi_note;
	};

#endif
