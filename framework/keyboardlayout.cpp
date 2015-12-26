#ifdef __WAND__
target[name[keyboardlayout.o] type[object]]
#endif

#include "keyboardlayout.h"
#include <cstring>
#include <algorithm>

static constexpr uint8_t s_keyboard_layout[]=
	{
	 1,  0, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 87, 88,  0,  0, 70,119,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	41,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,  0,110,102,104,  0, 69, 98, 55, 74,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,  0,111,107,109,  0, 71, 72, 73, 78,
	58, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 43, 28,  0,  0,  0,  0,  0, 75, 76, 77, 78,
	42, 86, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 54,  0,  0,103,  0,  0, 79, 80, 81, 96,
	29,125, 56, 57, 57, 57, 57, 57, 57, 57,100,126,127, 97,  0,105,108,106,  0, 82, 82, 83, 96
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_normal=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		 KeyDescriptor::VertexInternal{0,0}
		,KeyDescriptor::VertexInternal{16,0}
		,KeyDescriptor::VertexInternal{16,16}
		,KeyDescriptor::VertexInternal{0,16}
		,KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_backspace=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		 KeyDescriptor::VertexInternal{0,0}
		,KeyDescriptor::VertexInternal{29,0}
		,KeyDescriptor::VertexInternal{29,16}
		,KeyDescriptor::VertexInternal{0,16}
		,KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_tab=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		 KeyDescriptor::VertexInternal{0,0}
		,KeyDescriptor::VertexInternal{22,0}
		,KeyDescriptor::VertexInternal{22,16}
		,KeyDescriptor::VertexInternal{0,16}
		,KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_capslock=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		 KeyDescriptor::VertexInternal{0,0}
		,KeyDescriptor::VertexInternal{29,0}
		,KeyDescriptor::VertexInternal{29,16}
		,KeyDescriptor::VertexInternal{0,16}
		,KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_return=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		 KeyDescriptor::VertexInternal{0,0}
		,KeyDescriptor::VertexInternal{23,0}
		,KeyDescriptor::VertexInternal{23,32}
		,KeyDescriptor::VertexInternal{23-16,32}
		,KeyDescriptor::VertexInternal{23-16,16}
		,KeyDescriptor::VertexInternal{0,16}
		,KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_shift_l=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		 KeyDescriptor::VertexInternal{0,0}
		,KeyDescriptor::VertexInternal{19,0}
		,KeyDescriptor::VertexInternal{19,16}
		,KeyDescriptor::VertexInternal{0,16}
		,KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_shift_r=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		 KeyDescriptor::VertexInternal{0,0}
		,KeyDescriptor::VertexInternal{42,0}
		,KeyDescriptor::VertexInternal{42,16}
		,KeyDescriptor::VertexInternal{0,16}
		,KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_ctrl_l=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		 KeyDescriptor::VertexInternal{0,0}
		,KeyDescriptor::VertexInternal{26,0}
		,KeyDescriptor::VertexInternal{26,16}
		,KeyDescriptor::VertexInternal{0,16}
		,KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_ctrl_r=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		 KeyDescriptor::VertexInternal{0,0}
		,KeyDescriptor::VertexInternal{22-3,0}
		,KeyDescriptor::VertexInternal{22-3,16}
		,KeyDescriptor::VertexInternal{0,16}
		,KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_space=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		 KeyDescriptor::VertexInternal{0,0}
		,KeyDescriptor::VertexInternal{7*16,0}
		,KeyDescriptor::VertexInternal{7*16,16}
		,KeyDescriptor::VertexInternal{0,16}
		,KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr KeyboardLayout::KeyDescriptor
KeyboardLayout::s_key_skip=KeyDescriptor
	{
	ArrayFixed<KeyDescriptor::VertexInternal,8>
		{
		KeyDescriptor::VertexInternal{0xff,0xff}
		}
	,KeyDescriptor::ColorInternal{0,0,0,0xff}
	,KeyDescriptor::ColorInternal{0x80,0x80,0x80,0xff}
	,ArrayFixed<char,40>{}
	};

constexpr ArrayFixed<KeyboardLayout::KeyDescriptor
	,KeyboardLayout::TYPING_AREA_COLS*KeyboardLayout::TYPING_AREA_ROWS>
	KeyboardLayout::s_typing_area_default
	{
	 s_key_normal,s_key_normal,s_key_normal,s_key_normal,s_key_normal
	,s_key_normal,s_key_normal,s_key_normal,s_key_normal,s_key_normal
	,s_key_normal,s_key_normal,s_key_normal,s_key_backspace,s_key_skip

	,s_key_tab,s_key_normal,s_key_normal,s_key_normal,s_key_normal
	,s_key_normal,s_key_normal,s_key_normal,s_key_normal,s_key_normal
	,s_key_normal,s_key_normal,s_key_normal,s_key_return,s_key_skip

	,s_key_capslock,s_key_skip,s_key_normal,s_key_normal,s_key_normal
	,s_key_normal,s_key_normal,s_key_normal,s_key_normal,s_key_normal
	,s_key_normal,s_key_normal,s_key_normal,s_key_normal,s_key_skip

	,s_key_shift_l,s_key_normal,s_key_normal,s_key_normal,s_key_normal
	,s_key_normal,s_key_normal,s_key_normal,s_key_normal,s_key_normal
	,s_key_normal,s_key_normal,s_key_shift_r,s_key_skip,s_key_skip

	,s_key_ctrl_l,s_key_normal,s_key_normal,s_key_space,s_key_skip
	,s_key_skip,s_key_skip,s_key_skip,s_key_skip,s_key_skip
	,s_key_skip,s_key_normal,s_key_normal,s_key_normal,s_key_ctrl_r
	};

constexpr ArrayFixed<const char*
	,KeyboardLayout::TYPING_AREA_COLS*KeyboardLayout::TYPING_AREA_ROWS>
	KeyboardLayout::s_typing_area_labels_swe
	{
	 "§","1","2","3","4","5","6","7","8","9","0","+","`","⟵"," "
	,"⇋","Q","W","E","R","T","Y","U","I","O","P","Å","¨","↲"," "
	,"Caps"," ","A","S","D","F","G","H","J","K","L","Ö","Ä","'"," "
	,"⇑","<","Z","X","C","V","B","N","M",",",".","-","⇑"," "," "
	,"Ctrl","❖","Alt","Space"," "," "," "," "," "," "," ","Alt Gr","❖","❏","Ctrl"
	};

static constexpr uint8_t s_typing_area[]=
	{
	 0,  0, 41,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 14,
	 0, 15, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 28,
	 0, 58, 58, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 43, 28,  0,
	42, 42, 86, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 54, 54,  0,
	29, 29,125, 56, 57, 57, 57, 57, 57, 57, 57,100,126,127, 97, 97,  0
	};
static constexpr uint8_t s_ncols_keymap=17;

void KeyboardLayout::KeyDescriptor::labelSet(const char* text)
	{
	strncpy(label.begin(),text,label.length());
	*(label.end()-1)=0;
	}

uint8_t KeyboardLayout::scancodeFromCoordinates(float x,float y)
	{
	uint8_t row=uint8_t(y);
	uint8_t col=uint8_t(x-0.4f*row+2);
	if(row>4 || col>s_ncols_keymap-1)
		{return 0;}
	return s_typing_area[col+row*s_ncols_keymap];
	}

const uint8_t* KeyboardLayout::typingAreaScancodesBegin()
	{return s_typing_area;}

const uint8_t* KeyboardLayout::typingAreaScancodesEnd()
	{return s_typing_area+sizeof(s_typing_area);}

KeyboardLayout::KeyboardLayout()
	{
	memcpy(m_typing_area.begin(),s_typing_area_default.begin()
		,m_typing_area.size());

	auto ptr=m_typing_area.begin();
	auto ptr_end=m_typing_area.end();
	auto label=s_typing_area_labels_swe.begin();
	while(ptr!=ptr_end)
		{
		ptr->labelSet(*label);
		++label;
		++ptr;
		}

	ptr=m_typing_area.begin();
	m_from_scancode[41]=ptr+0;
	m_from_scancode[2]=ptr+1;
	m_from_scancode[3]=ptr+2;
	m_from_scancode[4]=ptr+3;
	m_from_scancode[5]=ptr+4;
	m_from_scancode[6]=ptr+5;
	m_from_scancode[7]=ptr+6;
	m_from_scancode[8]=ptr+7;
	m_from_scancode[9]=ptr+8;
	m_from_scancode[10]=ptr+9;
	m_from_scancode[11]=ptr+10;
	m_from_scancode[12]=ptr+11;
	m_from_scancode[13]=ptr+12;
	m_from_scancode[14]=ptr+13;
	++ptr;

	m_from_scancode[15]=ptr+14;
	m_from_scancode[16]=ptr+15;
	m_from_scancode[17]=ptr+16;
	m_from_scancode[18]=ptr+17;
	m_from_scancode[19]=ptr+18;
	m_from_scancode[20]=ptr+19;
	m_from_scancode[21]=ptr+20;
	m_from_scancode[22]=ptr+21;
	m_from_scancode[23]=ptr+22;
	m_from_scancode[24]=ptr+23;
	m_from_scancode[25]=ptr+24;
	m_from_scancode[26]=ptr+25;
	m_from_scancode[27]=ptr+26;
	m_from_scancode[28]=ptr+27;
	++ptr;

	m_from_scancode[58]=ptr+28;
	++ptr;
	m_from_scancode[30]=ptr+29;
	m_from_scancode[31]=ptr+30;
	m_from_scancode[32]=ptr+31;
	m_from_scancode[33]=ptr+32;
	m_from_scancode[34]=ptr+33;
	m_from_scancode[35]=ptr+34;
	m_from_scancode[36]=ptr+35;
	m_from_scancode[37]=ptr+36;
	m_from_scancode[38]=ptr+37;
	m_from_scancode[39]=ptr+38;
	m_from_scancode[40]=ptr+39;
	m_from_scancode[43]=ptr+40;
	++ptr;

	m_from_scancode[42]=ptr+41;
	m_from_scancode[86]=ptr+42;
	m_from_scancode[44]=ptr+43;
	m_from_scancode[45]=ptr+44;
	m_from_scancode[46]=ptr+45;
	m_from_scancode[47]=ptr+46;
	m_from_scancode[48]=ptr+47;
	m_from_scancode[49]=ptr+48;
	m_from_scancode[50]=ptr+49;
	m_from_scancode[51]=ptr+50;
	m_from_scancode[52]=ptr+51;
	m_from_scancode[53]=ptr+52;
	m_from_scancode[54]=ptr+53;
	ptr+=2;

	m_from_scancode[29]=ptr+54;
	m_from_scancode[125]=ptr+55;
	m_from_scancode[56]=ptr+56;
	m_from_scancode[57]=ptr+57;
	ptr+=7;
	m_from_scancode[100]=ptr+58;
	m_from_scancode[126]=ptr+59;
	m_from_scancode[127]=ptr+60;
	m_from_scancode[97]=ptr+61;

	}
