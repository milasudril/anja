//@	{
//@	"targets":[{"name":"aboutbox.hpp","type":"include"}]
//@	}

#ifndef ANJA_ABOUTBOX_HPP
#define ANJA_ABOUTBOX_HPP

#include "box.hpp"
#include "label.hpp"
#include "dialog.hpp"
#include "imageview.hpp"
#include "filler.hpp"
#include <memory>

namespace Anja
	{
	class AboutBox
		{
		struct LabelWrap
			{
			LabelWrap(Container& cnt,const char* text):m_label(cnt,text)
				{m_label.wordwrap(1);}
			Label m_label;
			};
		public:
			template<class Appinfo>
			AboutBox(Container& cnt,const Appinfo& info):
				 r_legal_brief(info.legalBrief())
				,r_techstring(info.techstring())
				,m_box(cnt,true)
					,m_logo(m_box.insertMode({0,Box::EXPAND|Box::FILL}))
					,m_filler_a(m_box.insertMode({2,0}))
					,m_title(m_box.insertMode({0,0}),info.nameAndRevision())
					,m_description(m_box.insertMode({0,0}),info.description())
					,m_copyright(m_box.insertMode({10,0}),info.copyright())
					,m_acknowlegement_title(m_box.insertMode({0,0}),"Acknowledgement")
					,m_acknowlegement(m_box.insertMode({0,0}),info.acknowledgementAll())
				{
				}

			enum class DialogId:int{LEGAL_BREIF,TECHSTRING};

			void legalBrief()
				{
				m_legal_brief.reset(new Dialog<LabelWrap,DialogOk>(m_box,"Legal Information"
					,r_legal_brief));
				m_legal_brief->callback(*this,DialogId::LEGAL_BREIF);
				}

			void techstring()
				{
				m_techstring.reset(new Dialog<LabelWrap,DialogOk>(m_box,"Technical Information"
					,r_techstring));
				m_techstring->callback(*this,DialogId::TECHSTRING);
				}

			void confirmPositive(Dialog<LabelWrap,DialogOk>& dlg,DialogId id)
				{
				switch(id)
					{
					case DialogId::LEGAL_BREIF:
						m_legal_brief.reset();
						break;
					case DialogId::TECHSTRING:
						m_techstring.reset();
						break;
					}
				}

			void logo(const ImageRepository& images,ImageRepository::IdType id
				,const std::pair<const uint8_t*,const uint8_t*>& data)
				{
				m_logo.minHeight(144).showPng(images,id,data);
				}

		private:
			const char* r_legal_brief;
			const char* r_techstring;
			Box m_box;
				ImageView m_logo;
				Filler m_filler_a;
				Label m_title;
				Label m_description;
				Label m_copyright;
				Label m_acknowlegement_title;
				Label m_acknowlegement;

			std::unique_ptr< Dialog<LabelWrap,DialogOk> > m_legal_brief;
			std::unique_ptr< Dialog<LabelWrap,DialogOk> > m_techstring;

		};
	}

#endif // ANJA_ABOUTBOX_HPP
