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
			SessionProperties& operator=(SessionProperties&&)=delete;
			SessionProperties(SessionProperties&&)=delete;

			SessionProperties(Container& cnt,Session& session);
			void sessionUpdated();

			enum class TextEntryId:int{TITLE};
			enum class SourceViewId:int{DESCRIPTION};
			enum class OptionListId:int{OPTIONS};

			void changed(TextEntry& entry,TextEntryId id);
			void changed(SourceView& entry,SourceViewId id);
			void clicked(OptionList& options,OptionListId id,Checkbox& option);

			template<class Callback,class IdType>
			SessionProperties& callback(Callback& cb,IdType id) noexcept
				{
				m_id=id;
				r_cb_obj=&cb;
				m_vtable=Vtable(cb,id);
				return *this;
				}

		private:
			struct Vtable
				{
				Vtable():title_changed(nullptr),description_changed(nullptr)
					,options_changed(nullptr)
					{}

				template<class Callback,class IdType>
				Vtable(Callback& cb_obj,IdType id)
					{
					title_changed=[](void* cb_obj,SessionProperties& self,int id)
						{reinterpret_cast<Callback*>(cb_obj)->titleChanged(self,static_cast<IdType>(id));};
					description_changed=[](void* cb_obj,SessionProperties& self,int id)
						{reinterpret_cast<Callback*>(cb_obj)->descriptionChanged(self,static_cast<IdType>(id));};
					options_changed=[](void* cb_obj,SessionProperties& self,int id)
						{reinterpret_cast<Callback*>(cb_obj)->optionsChanged(self,static_cast<IdType>(id));};
					}

				void (*title_changed)(void* cb_obj,SessionProperties& self,int id);
				void (*description_changed)(void* cb_obj,SessionProperties& self,int id);
				void (*options_changed)(void* cb_obj,SessionProperties& self,int id);
				};
			int m_id;
			void* r_cb_obj;
			Vtable m_vtable;

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
