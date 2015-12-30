#ifdef __WAND__
target[name[sessionview.h] type[include]]
dependency[sessionview.o]
#endif

#ifndef SESSIONVIEW_H
#define SESSIONVIEW_H

#include "framework/widget.h"
#include "waveformrangetrimmer.h"
#include "waveformdataview.h"
#include "framework/keyboardview.h"
#include "framework/keyboardlayout.h"

class GuiContainer;
class Session;
class KeyboardView;
class WaveformDataView;
class BoxVertical;

class SessionView:public Widget
	{
	public:
		static SessionView* create(GuiContainer& parent,Session& session);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void slotDisplayFromScancode(uint8_t scancode);

		void slotDisplayFromSlotIndex(uint8_t slot_index);

		void sessionSet(Session& session);

		void keyCurrentLabelSet(const char* label);

		void keyCurrentColorSet(const ColorRGBA& color_new);

		void keyCurrentColorSet(const char* colorstr);

	private:
		class KeyboardEventHandler:public KeyboardView::EventHandler
			{
			public:
				KeyboardEventHandler(SessionView& view):r_view(&view){}

				void onMouseUp(KeyboardView& source,uint8_t scancode
					,keymask_t key_mask)
					{
					r_view->slotDisplayFromScancode(scancode);
					source.update();
					}

				void onKeyDown(KeyboardView& source,uint8_t scancode)
					{}

			private:
				SessionView* r_view;
			};

		class WaveformDataEventHandler:public WaveformDataView::EventHandler
			{
			public:
				WaveformDataEventHandler(SessionView& view):r_view(&view){}
				void onSourceChange(WaveformDataView& source
					,const char* filename_new);
				void onDescriptionChange(WaveformDataView& source
					,const char* description_new);
				void onColorChange(WaveformDataView& source
					,const ColorRGBA& color_new);
				void onColorChange(WaveformDataView& source
					,const char* colorstr);
			private:
				SessionView* r_view;
			};

		SessionView(GuiContainer& parent,Session& session);
		~SessionView();

		Session* r_session;
		BoxVertical* m_box;
			KeyboardView* m_keyboard;
			WaveformDataView* m_dataview;

		WaveformRangeTrimmer m_trimmer;
		KeyboardEventHandler m_keyboardevents;
		WaveformDataEventHandler m_waveformevents;
		KeyboardLayout::KeyDescriptor* r_key_current;
	};

#endif
