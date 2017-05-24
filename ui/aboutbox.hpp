//@	{
//@	"targets":[{"name":"aboutbox.hpp","type":"include"}]
//@	}

#ifndef ANJA_ABOUTBOX_HPP
#define ANJA_ABOUTBOX_HPP

#include "box.hpp"
#include "label.hpp"

namespace Anja
	{
	class AboutBox
		{
		public:
			template<class Appinfo>
			AboutBox(Container& cnt,const Appinfo& info):
				m_box(cnt,true)
					,m_title(m_box,info.nameAndRevision())
					,m_description(m_box,info.description())
					,m_copyright(m_box.insertMode({4,0}),info.copyright())
					,m_acknowlegement_title(m_box.insertMode({2,0}),"Acknowledgement")
					,m_acknowlegement(m_box.insertMode({0,0}),info.acknowledgementAll())
					,m_legal_brief(m_box.insertMode({4,0}),info.legalBrief())
					,m_tech(m_box,info.techstring())
				{
				m_legal_brief.wordwrap(1);
				m_tech.wordwrap(1);
				}

		private:
			Box m_box;
				Label m_title;
				Label m_description;
				Label m_copyright;
				Label m_acknowlegement_title;
				Label m_acknowlegement;
				Label m_legal_brief;
				Label m_tech;

		};
	}

#endif // ANJA_ABOUTBOX_HPP
