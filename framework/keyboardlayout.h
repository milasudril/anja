#ifdef __WAND__
target[name[keyboardlayout.h] type[include]]
dependency[keyboardlayout.o]
#endif

#ifndef KEYBOARDLAYOUT_H
#define KEYBOARDLAYOUT_H

#include "color.h"
#include "array_fixed.h"

#include <cstdint>
#include <cstddef>
#include <initializer_list>

class KeyboardLayout
	{
	private:
	public:
		class alignas(32) KeyDescriptor
			{
			friend class KeyboardLayout;
			public:
				constexpr KeyDescriptor(const KeyDescriptor& key)=default;
				constexpr KeyDescriptor():label{0}
					{}
				constexpr KeyDescriptor(char x):label{x}{}

				struct Vertex
					{
					float x;
					float y;
					};

				Color colorGet() const
					{
					return
						{
						 (float)m_color.r/255
						,(float)m_color.g/255
						,(float)m_color.b/255
						,(float)m_color.a/255
						};
					}

				Color colorBorderGet() const
					{
					return
						{
						 (float)m_color_border.r/255
						,(float)m_color_border.g/255
						,(float)m_color_border.b/255
						,(float)m_color_border.a/255
						};
					}


				void colorSet(const ColorRGBA& color)
					{
					m_color.r=color.red*255;
					m_color.g=color.green*255;
					m_color.b=color.blue*255;
					m_color.a=color.alpha*255;
					}

				void colorBorderSet(const Color& color)
					{
					m_color_border.r=color.red*255;
					m_color_border.g=color.green*255;
					m_color_border.b=color.blue*255;
					m_color_border.a=color.alpha*255;
					}

				Vertex vertexGet(uint8_t index) const
					{
					return
						vertices[index].x==0xff?
							 Vertex{-1.0f,-1.0f}
							:Vertex{
							  float(vertices[index].x)/16
							 ,float(vertices[index].y)/16
							};
					}

				void labelSet(const char* str);

				const char* labelGet() const
					{return label.begin();}


			private:
				struct VertexInternal
					{
					constexpr VertexInternal(uint8_t _x,uint8_t _y):
						x(_x),y(_y)
						{}

					constexpr VertexInternal(uint8_t _x):x(_x),y(_x){}

					constexpr VertexInternal():x(0),y(0){}
					uint8_t x;
					uint8_t y;
					};

				struct alignas(4) ColorInternal
					{
					constexpr ColorInternal(uint8_t _r,uint8_t _g,uint8_t _b,uint8_t _a)
						:r(_r),g(_g),b(_b),a(_a)
						{}
					constexpr ColorInternal():r(0),g(0),b(0),a(0){}
					uint8_t r;
					uint8_t g;
					uint8_t b;
					uint8_t a;
					};

				constexpr KeyDescriptor(const ArrayFixed<VertexInternal,8> v
					,const ColorInternal& c
					,const ColorInternal& c_border
					,const ArrayFixed<char,40>& l):
					vertices(v),m_color(c),m_color_border(c_border),label(l)
					{}

				ArrayFixed<VertexInternal,8> vertices;
				ColorInternal m_color;
				ColorInternal m_color_border;
				ArrayFixed<char,40> label;
			};

		KeyboardLayout();

		static constexpr size_t typingAreaCols()
			{return TYPING_AREA_COLS;}

		static constexpr size_t typingAreaRows()
			{return TYPING_AREA_ROWS;}

		const KeyDescriptor* typingAreaBegin() const
			{return m_typing_area.begin();}

		const KeyDescriptor* typingAreaEnd() const
			{return m_typing_area.end();}

		const KeyDescriptor* keyFromScancode(uint8_t scancode) const
			{return m_from_scancode[scancode];}

		KeyDescriptor* keyFromScancode(uint8_t scancode)
			{return m_from_scancode[scancode];}

		static uint8_t scancodeFromCoordinates(float x,float y);

		static const uint8_t* typingAreaScancodesBegin();

		static const uint8_t* typingAreaScancodesEnd();

	private:
		static constexpr uint8_t TYPING_AREA_COLS=15;
		static constexpr uint8_t TYPING_AREA_ROWS=5;

		static const KeyDescriptor s_key_normal;
		static const KeyDescriptor s_key_backspace;
		static const KeyDescriptor s_key_tab;
		static const KeyDescriptor s_key_capslock;
		static const KeyDescriptor s_key_return;
		static const KeyDescriptor s_key_shift_l;
		static const KeyDescriptor s_key_shift_r;
		static const KeyDescriptor s_key_ctrl_l;
		static const KeyDescriptor s_key_space;
		static const KeyDescriptor s_key_ctrl_r;
		static const KeyDescriptor s_key_skip;

		static const ArrayFixed<KeyDescriptor,TYPING_AREA_COLS*TYPING_AREA_ROWS>
			s_typing_area_default;
		static const ArrayFixed<const char*,TYPING_AREA_COLS*TYPING_AREA_ROWS>
			s_typing_area_labels_swe;

		ArrayFixed<KeyDescriptor,TYPING_AREA_COLS*TYPING_AREA_ROWS>
			m_typing_area;

		ArrayFixed<KeyDescriptor*,256> m_from_scancode;
	};

#endif
