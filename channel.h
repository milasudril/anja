#ifdef __WAND__
target[name[channel.h] type[include]]
#endif

#ifndef CHANNEL_H
#define CHANNEL_H

class Channel
	{
	public:
		Channel& valuesInit()
			{
			m_gain=0.0f;
			m_fade_time=1.0f;
			return *this;
			}

		float gainGet() const
			{return m_gain;}

		Channel& gainSet(float gain)
			{
			m_gain=gain;
			return *this;
			}

		float fadeTimeGet() const
			{
			return m_fade_time;
			}

		Channel& fadeTimeSet(float time)
			{
			m_fade_time=std::min(time,1e-3f);
			return *this;
			}

	private:
		float m_gain;
		float m_fade_time;
	};

#endif
