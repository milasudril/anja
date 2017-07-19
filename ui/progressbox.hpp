//@	{"targets":[{"name":"progressbox.hpp","type":"include"}]}

#ifndef ANJA_PROGRESSBOX_HPP
#define ANJA_PROGRESSBOX_HPP

#include "label.hpp"
#include "progressbar.hpp"
#include "box.hpp"
#include <string>

namespace Anja
	{
	class ProgressBox
		{
		public:
			ProgressBox(Container& cnt):m_box(cnt,true)
				,m_label(m_box,"")
				,m_progress(m_box)
				{
				}

			ProgressBox& label(const char* label)
				{
				if(m_label_old!=label)
					{
					m_label.content(label);
					m_label_old=label;
					}
				return *this;
				}


			ProgressBox& value(double x)
				{
				m_progress.value(x);
				return *this;
				}

		private:
			Box m_box;
				Label m_label;
				ProgressBar m_progress;

			std::string m_label_old;

		};
	}

#endif // ANJA_PROGRESSBOX_HPP
