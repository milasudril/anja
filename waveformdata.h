#ifdef __WAND__
target[name[waveformdata.h] type[include]]
dependency[waveformdata.o]
#endif

#ifndef WAVEFORMDATA_H
#define WAVEFORMDATA_H

#include "waveform.h"

#include "framework/color.h"
#include "framework/keyboardlayout.h"
#include <string>

class SessionFileRecord;
class ColorString;

class WaveformData
	{
	public:
		WaveformData();

		WaveformData(const SessionFileRecord& record
			,const ArrayDynamicShort<char>& load_path
			,Waveform& storage
			,KeyboardLayout::KeyDescriptor* key);

		void init(Waveform& storage,KeyboardLayout::KeyDescriptor* key);

		Waveform& waveformGet() noexcept
			{
			return *r_waveform;
			}

		const ArrayDynamicShort<char>& filenameGet() const
			{return m_filename;}

		void fileLoad(const ArrayDynamicShort<char>& filename
			,const ArrayDynamicShort<char>& load_path);

		void fileLoad(const char* filename);

		void fileSave(unsigned int k,const ArrayDynamicShort<char>& path);



		const ArrayDynamicShort<char>& descriptionGet() const noexcept
			{return m_description;}

		void descriptionSet(const ArrayDynamicShort<char>& description);

		void descriptionSet(const char* description);

		const ArrayDynamicShort<char>& keyLabelGet() const noexcept
			{return m_key_label;}



		const ColorRGBA& keyColorGet() const noexcept
			{return m_color;}

		void keyColorSet(const ColorRGBA& color_new);

		void dataGet(SessionFileRecord& record
			,const ArrayDynamicShort<char>& load_path) const;

		bool dirtyIs() const noexcept
			{
			return m_stateflags&DIRTY || r_waveform->dirtyIs();
			}

		void dirtyClear()
			{
			m_stateflags&=~DIRTY;
			r_waveform->dirtyClear();
			}

	private:
		ArrayDynamicShort<char> m_filename;
		ArrayDynamicShort<char> m_description;
		ArrayDynamicShort<char> m_key_label;
		ColorRGBA m_color;
		KeyboardLayout::KeyDescriptor* r_key;
		Waveform* r_waveform;

		static constexpr unsigned int DIRTY=0x1;

		unsigned int m_stateflags;
	};

#endif
