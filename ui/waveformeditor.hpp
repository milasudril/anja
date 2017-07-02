//@	{
//@	"targets":[{"name":"waveformeditor.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"waveformeditor.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFORMEDITOR_HPP
#define ANJA_WAVEFORMEDITOR_HPP

#include "box.hpp"
#include "label.hpp"
#include "textentry.hpp"
#include "paned.hpp"
#include "combobox.hpp"
#include "button.hpp"
#include "optionlist.hpp"
#include "slider.hpp"
#include "xyplot.hpp"
#include "dialog.hpp"
#include "colorpicker.hpp"
#include "message.hpp"
#include "separator.hpp"
#include "../sessiondata/waveformproxy.hpp"
#include "../common/arraysimple.hpp"

namespace Anja
	{
	class Session;

	template<class T,class SizeType>
	class ArraySimple;

	class WaveformEditor
		{
		public:
			struct DialogConfirmSave
				{
				static constexpr const char* dismiss() noexcept
					{return "Cancel";}

				static constexpr const char* confirmPositive() noexcept
					{return "Save";}

				static constexpr const char* confirmNegative() noexcept
					{return "Don't save";}

				static constexpr const char* user1() noexcept
					{return nullptr;}

				static constexpr const char* user2() noexcept
					{return nullptr;}
				};


			enum class ButtonId:int
				{FILENAME_BROWSE,FILENAME_RELOAD,COLOR_PICK,CURSORS_SWAP};

			enum class ListboxId:int
				{CHANNEL};

			enum class SliderId:int
				{GAIN,GAIN_RANDOM};

			enum class TextEntryId:int
				{
				 FILENAME,DESCRIPTION,COLOR,GAIN,GAIN_RANDOM,CURSOR_BEGIN
				,CURSOR_BEGIN_LOOP,CURSOR_END_LOOP,CURSOR_END
				};

			enum class OptionListId:int
				{OPTIONS};

			enum class PlotId:int
				{WAVEFORM};


			WaveformEditor& operator=(WaveformEditor&&)=delete;
			WaveformEditor(WaveformEditor&&)=delete;

			template<class StringRange>
			WaveformEditor(Container& cnt,const ImageRepository& images,const WaveformProxy& waveform
				,const StringRange& channel_names):
				WaveformEditor(cnt,images,waveform,channel_names.begin(),channel_names.end())
				{}

			WaveformEditor(Container& cnt,ImageRepository&& images,const WaveformProxy& waveform
				,const String* channel_names_begin
				,const String* channel_names_end)=delete;

			WaveformEditor(Container& cnt,const ImageRepository& images,const WaveformProxy& waveform
				,const String* channel_names_begin
				,const String* channel_names_end);

			template<class ColorRange>
			WaveformEditor& colorPresets(const ColorRange& colors)
				{return colorPresets(colors.begin(),colors.end());}

			WaveformEditor& colorPresets(const ColorRGBA* begin,const ColorRGBA* end);

			WaveformEditor& channelName(int index,const char* name);

			template<class StringRange>
			WaveformEditor& channelNames(const StringRange& channel_names)
				{return channelNames(channel_names.begin(),channel_names.end());}

			WaveformEditor& channelNames(const String* names_begin,const String* names_end);

			template<class Callback,class IdType>
			WaveformEditor& callback(Callback& cb,IdType id) noexcept
				{
				m_id=static_cast<int>(id);
				r_cb_obj=&cb;
				m_vtable=Vtable(cb,id);
				return *this;
				}

			void clicked(Button& src,ButtonId id);
			void clicked(OptionList& src,OptionListId id,Checkbox& opt);
			void changed(Slider& slider,SliderId id);
			void changed(TextEntry& entry,TextEntryId id);
			void changed(Combobox& lb,ListboxId id);
			void cursorXMove(XYPlot& plot,PlotId id,int index,keymask_t keymask);
			void cursorYMove(XYPlot& plot,PlotId id,int index,keymask_t keymask);
			void cursorXRightclick(XYPlot& plot,PlotId id,int index,keymask_t keymask);
			void cursorYRightclick(XYPlot& plot,PlotId id,int index,keymask_t keymask);

			void dismiss(Dialog<ColorPicker>& dlg,int id);
			void confirmPositive(Dialog<ColorPicker>& dlg,int id);
			void confirmPositive(Dialog<Message,DialogOk>& dlg,int id);


			void dismiss(Dialog<Message,DialogConfirmSave>& dlg,int id);
			void confirmPositive(Dialog<Message,DialogConfirmSave>& dlg,int id);
			void confirmNegative(Dialog<Message,DialogConfirmSave>& dlg,int id);

			WaveformEditor& waveform(const WaveformProxy& waveform);
			WaveformEditor& waveformUpdate();

		private:
			struct Vtable
				{
				Vtable():description_changed(nullptr),color_changed(nullptr),color_presets_changed(nullptr)
					{}

				template<class Callback,class IdType>
				Vtable(Callback& cb_obj,IdType id)
					{
					description_changed=[](void* cb_obj,WaveformEditor& self,int id)
						{reinterpret_cast<Callback*>(cb_obj)->descriptionChanged(self,static_cast<IdType>(id));};
					color_changed=[](void* cb_obj,WaveformEditor& self,int id)
						{reinterpret_cast<Callback*>(cb_obj)->colorChanged(self,static_cast<IdType>(id));};
					color_presets_changed=[](void* cb_obj,ColorPicker& self)
						{reinterpret_cast<Callback*>(cb_obj)->colorPresetsChanged(self);};
					}

				void (*description_changed)(void* cb_obj,WaveformEditor& self,int id);
				void (*color_changed)(void* cb_obj,WaveformEditor& self,int id);
				void (*color_presets_changed)(void* cb_obj,ColorPicker& self);
				};

			int m_id;
			void* r_cb_obj;
			Vtable m_vtable;

			void offsets_update();
			void cursor_begin_auto() noexcept;
			void cursor_end_auto() noexcept;
			bool waveform_saved();
			void waveform_confirm_load(int method);
			void waveform_load(int method);
			void waveform_load(const char* filename_new);

			WaveformProxy m_waveform;
			ArraySimple<float> m_waveform_db;
			std::unique_ptr<Dialog<ColorPicker>> m_color_dlg;
			std::unique_ptr<Dialog<Message,DialogOk>> m_err_dlg;
			std::unique_ptr<Dialog<Message,DialogConfirmSave>> m_confirm_dlg;
			const ColorRGBA* r_color_presets_begin;
			const ColorRGBA* r_color_presets_end;
			Box m_box;
				Box m_filename;
					Label m_filename_label;
					TextEntry m_filename_input;
					Button m_filename_browse;
					Button m_filename_reload;
				Box m_description;
					Label m_description_label;
					TextEntry m_description_input;
				Paned m_details;
					Box m_details_left;
						Box m_color;
							Label m_color_label;
							TextEntry m_color_input;
							Button m_color_pick;
						Box m_channel;
							Label m_channel_label;
							Combobox m_channel_input;
						Box m_gain;
							Label m_gain_label;
							Box m_gain_input;
								TextEntry m_gain_input_text;
								Slider m_gain_input_slider;
						Box m_gain_random;
							Label m_gain_random_label;
							Box m_gain_random_input;
								TextEntry m_gain_random_input_text;
								Slider m_gain_random_input_slider;
						Box m_options;
							Label m_options_label;
						OptionList m_options_input;
					Box m_details_right;
						XYPlot m_plot;
						Box m_trim_panel;
							Box m_cursor_begin;
								Label m_cursor_begin_label;
								TextEntry m_cursor_begin_entry;
							Separator m_trim_sep_a;
							Box m_cursor_begin_loop;
								Label m_cursor_begin_loop_label;
								TextEntry m_cursor_begin_loop_entry;
							Separator m_trim_sep_b;
							Button m_swap;
							Separator m_trim_sep_c;
							Box m_cursor_end_loop;
								TextEntry m_cursor_end_loop_entry;
								Label m_cursor_end_loop_label;
							Separator m_trim_sep_d;
							Box m_cursor_end;
								TextEntry m_cursor_end_entry;
								Label m_cursor_end_label;
			const ImageRepository& r_images;
		};

	}

#endif // ANJA_WAVEFORMEDITOR_HPP
