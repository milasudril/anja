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
#include "sourceview.hpp"

namespace Anja
	{
	class Session;
	class SessionProperties
		{
		public:
			SessionProperties(Container& cnt,Session& session);
			void sessionUpdated();

			enum class TextEntryId:int{TITLE};
			enum class SourceViewId:int{DESCRIPTION};
			enum class OptionListId:int{OPTIONS};

			void changed(TextEntry& entry,TextEntryId id);
			void changed(SourceView& entry,SourceViewId id);
			void clicked(OptionList& options,OptionListId id,Checkbox& option);

		private:
			Session* r_session;
			Box m_box;
				Box m_title;
					Label m_title_label;
					TextEntry m_title_input;
				Box m_description;
					Label m_description_label;
				SourceView m_description_input;
				Box m_options;
					Label m_options_label;
				OptionList m_options_input;
		};
	}

#endif // ANJA_SESSIONPROPERTIES_HPP
