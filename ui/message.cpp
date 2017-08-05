//@	{
//@	 "targets":
//@		[{
//@		"name":"message.o","type":"object"
//@		}]
//@	}


#include "message.hpp"
#include "statusicons.hpp"

using namespace Anja;

static void imageShow(ImageView& view,const ImageRepository& images,StatusIcon id)
	{view.showPng(images,static_cast<ImageRepository::IdType>(id),statusIcon(id));}

Message::Message(Container& cnt,const ImageRepository& images,const char* message
	,Type t,bool wordwrap):
	 m_cols(cnt,false)
		,m_icon(m_cols.insertMode({2,0}))
		,m_text(m_cols.insertMode(
			{2,static_cast<unsigned short>(wordwrap?(Box::EXPAND|Box::FILL):0)})
			,message)
	,r_images(images)
	{
	if(!wordwrap)
		{m_icon.minHeight(18);}
	type(t);
	}

Message& Message::type(Type type)
	{
	switch(type)
		{
		case Type::ERROR:
			imageShow(m_icon,r_images,StatusIcon::ERROR);
			break;
		case Type::USER_ERROR:
			imageShow(m_icon,r_images,StatusIcon::USER_ERROR);
			break;
		case Type::INFORMATION:
			imageShow(m_icon,r_images,StatusIcon::INFORMATION);
			break;
		case Type::WARNING:
			imageShow(m_icon,r_images,StatusIcon::WARNING);
			break;
		case Type::STOP:
			imageShow(m_icon,r_images,StatusIcon::STOP);
			break;
		case Type::WAIT:
			imageShow(m_icon,r_images,StatusIcon::WAIT);
			break;
		case Type::READY:
			imageShow(m_icon,r_images,StatusIcon::READY);
			break;

		}
	return *this;
	}

Message& Message::message(const char* msg)
	{
	m_text.content(msg);
	return *this;
	}
