//@	{
//@	"targets":
//@		[{
//@		 "name":"message.hpp","type":"include"
//@		,"include_targets":["controlcodes.hpp","statuscodes.hpp"]
//@		}]
//@	}

#ifndef ANJA_MIDI_MESSAGE_HPP
#define ANJA_MIDI_MESSAGE_HPP

#include <cstdint>

namespace Anja
	{
	namespace MIDI
		{
		class Message
			{
			public:
				Message() noexcept
					{m_content.data=0;}

				Message(ControlCodes ctrl_val,int channel,int value) noexcept
					{
					m_content.data=0;
					m_content.bytes[0]=channel|static_cast<uint8_t>(MIDI::StatusCodes::CONTROL_CHANGE);
					m_content.bytes[1]=static_cast<uint8_t>(ctrl_val);
					m_content.bytes[2]=value;
					m_content.bytes[3]=1;
					}

				Message(ControlCodes ctrl_val,int value) noexcept
					{
					m_content.data=0;
					m_content.bytes[0]=static_cast<uint8_t>(MIDI::StatusCodes::CONTROL_CHANGE);
					m_content.bytes[1]=static_cast<uint8_t>(ctrl_val);
					m_content.bytes[2]=value;
					}

				Message(StatusCodes status,int channel,int v1,int v2) noexcept
					{
					m_content.data=0;
					m_content.bytes[0]=channel|static_cast<uint8_t>(status);
					m_content.bytes[1]=v1;
					m_content.bytes[2]=v2;
					m_content.bytes[3]=1;
					}

				Message(StatusCodes status,int v1,int v2) noexcept
					{
					m_content.data=0;
					m_content.bytes[0]=static_cast<uint8_t>(status);
					m_content.bytes[1]=v1;
					m_content.bytes[2]=v2;
					}

				StatusCodes status() const noexcept
					{
					if(m_content.bytes[3])
						{return static_cast<StatusCodes>(m_content.bytes[0]&0xf);}
					return static_cast<StatusCodes>(m_content.bytes[0]);
					}

				ControlCodes ctrlCode() const noexcept
					{return static_cast<ControlCodes>( m_content.bytes[1] );}

				int channel() const noexcept
					{
					if(m_content.bytes[3])
						{return m_content.bytes[0]&0xf0;}
					return -1;
					}

				int value1() const noexcept
					{return m_content.bytes[1];}

				int value2() const noexcept
					{return m_content.bytes[2];}

				uint8_t& bytes(int k) noexcept
					{return m_content.bytes[k];}


			private:
				union Content
					{
					int32_t data;
					uint8_t bytes[4];
					} m_content;
			};
		}
	}

#endif // ANJA_MESSAGE_HPP
