//@	{
//@	 "targets":[{"name":"sessionproperties.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sessionproperties.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SESSIONPROPERTIES_HPP
#define ANJA_SESSIONPROPERTIES_HPP

#include "box.hpp"
#include "label.hpp"
#include "textentry.hpp"
#include "optionlist.hpp"

namespace Anja
	{
	class Session;
	class SessionProperties
		{
		public:
			SessionProperties(Container& cnt,Session& session);

		private:
			Session* r_session;
			Box m_box;
				Box m_title;
					Label m_title_label;
					TextEntry m_title_input;
				Box m_description;
					Label m_description_label;
					TextEntry m_description_input;
				Box m_options;
					Label m_options_label;
					OptionList m_options_input;
		};
	}

#endif // ANJA_SESSIONPROPERTIES_HPP
