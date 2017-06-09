//@	{
//@	 "targets":[{"name":"message.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"message.o","rel":"implementation"}]
//@	}


#ifndef ANJA_MESSAGE_HPP
#define ANJA_MESSAGE_HPP

#include "label.hpp"
#include "box.hpp"
#include "imageview.hpp"

namespace Anja
	{
	class Message
		{
		public:
			enum class Type:int{INFORMATION,WARNING,ERROR};

			Message(Container& cnt,const char* message,Type type,bool wordwrap=true);

		private:
			Box m_cols;
				ImageView m_icon;
				Label m_text;
		};
	}

#endif // ANJA_MESSAGE_HPP
