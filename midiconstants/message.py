#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"message.hpp","dependencies":
#@				[
#@				 {"ref":"statuscodes.hpp","rel":"generated"}
#@				,{"ref":"controlcodes.hpp","rel":"generated"}
#@				,{"ref":"gmdrumkit.hpp","rel":"generated"}
#@				,{"ref":"gmprograms.hpp","rel":"generated"}
#@				]
#@			 }
#@      ]
#@  }

import sys

file_content='''#ifndef ANJA_MIDI_MESSAGE_HPP
#define ANJA_MIDI_MESSAGE_HPP

#include "statuscodes.hpp"
#include "controlcodes.hpp"
#include "gmdrumkit.hpp"
#include "gmprograms.hpp"

namespace Anja
	{
	namespace MIDI
		{
		class Message
			{
			public:
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
						{return static_cast<MIDI::StatusCodes>(m_content.bytes[0]&0xf);}
					return static_cast<MIDI::StatusCodes>(m_content.bytes[0]);
					}

				ControlCodes ctrlCode() const noexcept
					{return static_cast<MIDI::ControlCodes>( m_content.bytes[1] );}

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


			private:
				union Content
					{
					int32_t data;
					uint8_t bytes[4];
					} m_content;
			};
		}
	}

#endif'''

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

try:
	target_dir=sys.argv[1];
	in_dir=sys.argv[2];
	with open(target_dir + '/' + in_dir + '/message.hpp','wb') as output:
		output.write(file_content.encode('utf-8'))

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
