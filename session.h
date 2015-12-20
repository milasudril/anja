#ifdef __WAND__
target[name[session.h] type[include]]
#endif

#ifndef SESSION_H
#define SESSION_H

class Session
	{
	public:
		class WaveformData
			{
			private:
				std::string m_filename;
				std::string m_description;
				uint64_t m_position_start;
				uint64_t m_position_stop;
				float m_playback_gain;
				uint32_t m_flags;
				uint8_t m_scancode;
				uint8_t m_midi_note;

				static constexpr uint32_t READONLY=1;
				static constexpr uint32_t GAIN_RANDOMIZE=2;
				static constexpr uint32_t PITCH_RANDOMIZE=4;
				static constexpr uint32_t LOOP=8;
				static constexpr uint32_t SUSTAIN=16;
			};

	private:
		std::string m_directory;
		float m_gain;
		std::vector<WaveformData> keylist;

		static constexpr uint32_t ENGINE_ONLINE=1;
		static constexpr uint32_t MODE_RECORD=2;
		static constexpr uint32_t VIEW_LIGHT=4;
	};

#endif
