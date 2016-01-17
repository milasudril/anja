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
			m_decay_time=1.0f;
			return *this;
			}

		float gainGet() const
			{return m_gain;}

		Channel& gainSet(float gain)
			{
			m_gain=gain;
			return *this;
			}

		Channel& decayTimeSet(float time)
			{
			m_decay_time=time;
			return *this;
			}

	private:
		float m_gain;
		float m_decay_time;
	};

#endif
