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
			enum class Type:int{INFORMATION,WARNING,ERROR,READY,WAIT,STOP};

			Message(Container& cnt,ImageRepository&& images,const char* message
				,Type type,bool wordwrap=true)=delete;

			Message(Container& cnt,const ImageRepository& images,const char* message
				,Type type,bool wordwrap=true);

			Message& type(Type t);

			Message& message(const char* msg);

		private:
			Box m_cols;
				ImageView m_icon;
				Label m_text;
			const ImageRepository& r_images;
		};
	}

#endif // ANJA_MESSAGE_HPP
