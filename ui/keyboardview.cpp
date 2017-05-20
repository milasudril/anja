//@	{
//@	"targets":[{"type":"object","name":"keyboardview.o","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "keyboardview.hpp"
#include "container.hpp"
#include "../common/arrayfixed.hpp"
#include "../common/vec2.hpp"
#include "../common/color.hpp"
#include <gtk/gtk.h>

using namespace Anja;

namespace
	{
	struct KeyPolygonVertex
		{
		constexpr KeyPolygonVertex(uint8_t _x,uint8_t _y):
			x(_x),y(_y)
			{}

		constexpr KeyPolygonVertex(uint8_t _x):x(_x),y(_x){}

		constexpr KeyPolygonVertex():x(0),y(0){}
		uint8_t x;
		uint8_t y;

		Vec2 normalize() const noexcept
			{return Vec2{static_cast<double>(x)/16.0,static_cast<double>(y)/16.0};}
		};

	class KeyPolygon
		{
		public:
			template<class ... T>
			constexpr KeyPolygon(T ... verts) noexcept:
				m_data({1,sizeof...(verts)+1},verts...)
				{}

			constexpr const KeyPolygonVertex* begin() const noexcept
				{return m_data.begin() + (m_data.begin()->x);}

			constexpr const KeyPolygonVertex* end() const noexcept
				{return m_data.begin() + (m_data.begin()->y);}

			constexpr size_t size() const noexcept
				{return end() - begin();}


		private:
			ArrayFixed<KeyPolygonVertex,8> m_data;
		};
	}

constexpr KeyPolygon s_key_normal
	{
	 KeyPolygonVertex{0,0}
	,KeyPolygonVertex{16,0}
	,KeyPolygonVertex{16,16}
	,KeyPolygonVertex{0,16}
	};

constexpr KeyPolygon s_key_backspace
	{
	 KeyPolygonVertex{0,0}
	,KeyPolygonVertex{29,0}
	,KeyPolygonVertex{29,16}
	,KeyPolygonVertex{0,16}
	};

constexpr KeyPolygon s_key_tab
	{
	 KeyPolygonVertex{0,0}
	,KeyPolygonVertex{22,0}
	,KeyPolygonVertex{22,16}
	,KeyPolygonVertex{0,16}
	};

constexpr KeyPolygon s_key_capslock
	{
	 KeyPolygonVertex{0,0}
	,KeyPolygonVertex{29,0}
	,KeyPolygonVertex{29,16}
	,KeyPolygonVertex{0,16}
	};

constexpr KeyPolygon s_key_return
	{
	 KeyPolygonVertex{0,0}
	,KeyPolygonVertex{23,0}
	,KeyPolygonVertex{23,32}
	,KeyPolygonVertex{23-16,32}
	,KeyPolygonVertex{23-16,16}
	,KeyPolygonVertex{0,16}
	};

constexpr KeyPolygon s_key_shift_l
	{
	 KeyPolygonVertex{0,0}
	,KeyPolygonVertex{19,0}
	,KeyPolygonVertex{19,16}
	,KeyPolygonVertex{0,16}
	};

constexpr KeyPolygon s_key_shift_r
	{
	 KeyPolygonVertex{0,0}
	,KeyPolygonVertex{42,0}
	,KeyPolygonVertex{42,16}
	,KeyPolygonVertex{0,16}
	};

constexpr KeyPolygon s_key_ctrl_l
	{
	 KeyPolygonVertex{0,0}
	,KeyPolygonVertex{26,0}
	,KeyPolygonVertex{26,16}
	,KeyPolygonVertex{0,16}
	};

constexpr KeyPolygon s_key_ctrl_r
	{
	 KeyPolygonVertex{0,0}
	,KeyPolygonVertex{22-3,0}
	,KeyPolygonVertex{22-3,16}
	,KeyPolygonVertex{0,16}
	};

constexpr KeyPolygon s_key_space
	{
	 KeyPolygonVertex{0,0}
	,KeyPolygonVertex{7*16,0}
	,KeyPolygonVertex{7*16,16}
	,KeyPolygonVertex{0,16}
	};

constexpr KeyPolygon s_key_skip
	{
	};



static constexpr uint8_t TYPING_AREA_COLS=15;
static constexpr uint8_t TYPING_AREA_ROWS=5;

constexpr ArrayFixed<KeyPolygon,TYPING_AREA_COLS*TYPING_AREA_ROWS> s_typing_area
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

constexpr uint8_t s_typing_area_scancodes[]=
	{
	 41,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,  12, 13, 14, 14
	,15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  26, 27, 28, 28
	,58, 58, 30, 31, 32, 33, 34, 35, 36, 37, 38,  39, 40, 43, 28
	,42, 86, 44, 45, 46, 47, 48, 49, 50, 51, 52,  53, 54, 54, 54
	,29,125, 56, 57, 57, 57, 57, 57, 57, 57, 57, 100,126,127, 97
	};

static constexpr ArrayFixed<uint8_t,256> gen_scancode_typing_area_keys()
	{
	ArrayFixed<uint8_t,256> ret;
	for(decltype(ret.length()) k=0;k<ret.length();++k)
		{
		uint8_t index=0xff;
		for(int l=0;l<TYPING_AREA_COLS*TYPING_AREA_ROWS;++l)
			{
			if(s_typing_area_scancodes[l]==k)
				{
				index=l;
				break;
				}
			}
		ret[k]=index;
		}
	return ret;
	}

constexpr ArrayFixed<uint8_t,256> s_scancode_typing_area_keys=gen_scancode_typing_area_keys();

static constexpr uint8_t scancode_typing_area_keys(uint8_t scancode)
	{
	return s_scancode_typing_area_keys[scancode];
	}

static constexpr uint8_t x_max(const KeyPolygon& p)
	{
	if(p.size()==0)
		{return 0;}
	auto ptr=p.begin();
	auto ptr_end=p.end();
	auto ret=ptr->x;
	++ptr;
	while(ptr!=ptr_end)
		{
		ret=std::max(ptr->x,ret);
		++ptr;
		}
	return ret;
	}

template<size_t N>
static constexpr auto x_positions(const ArrayFixed<KeyPolygon,N>& keys,size_t n_cols)
	{
	ArrayFixed<uint8_t,N> ret;
	ret[0]=0;
	for(size_t k=1;k<N;++k)
		{ret[k]=k%n_cols==0?0:ret[k-1]+x_max(keys[k-1]);}
	return ret;
	}

static constexpr auto s_typing_area_x_pos=x_positions(s_typing_area,TYPING_AREA_COLS);

static constexpr auto FUNCTION_KEYS_COLS=12;


constexpr ArrayFixed<KeyPolygon,FUNCTION_KEYS_COLS> s_function_keys
	{
	 s_key_normal,s_key_normal,s_key_normal,s_key_normal
	,s_key_normal,s_key_normal,s_key_normal,s_key_normal
	,s_key_normal,s_key_normal,s_key_normal,s_key_normal
	};

static constexpr uint8_t s_function_keys_scancodes[]
	{
	 59,60,61,62
	,63,64,65,66
	,67,68,87,88
	};

static constexpr uint8_t scancode_function_keys(uint8_t scancode)
	{
	if(scancode>=59 && scancode<69)
		{return scancode - 59;}
	if(scancode>=87 && scancode<88)
		{return 10 + (scancode - 87);}
	return 0xff;
	}

static constexpr auto s_function_keys_x_pos=x_positions(s_function_keys,FUNCTION_KEYS_COLS);

std::pair<KeyboardView::KeyType,int> KeyboardView::keyType(int scancode) const noexcept
	{
	auto index=scancode_function_keys(scancode);
	if(index!=0xff)
		{return {KeyType::FUNCTION_KEY,index};}

	index=scancode_typing_area_keys(scancode);
	if(index!=0xff)
		{return {KeyType::TYPING_KEY,index};}
	return {KeyType::OTHER,0xff};
	}

bool KeyboardView::modifier(int scancode) const noexcept
	{return scancode==42 || scancode==29 || scancode==56 || scancode==100 || scancode==97;}




class KeyboardView::Impl:public KeyboardView
	{
	public:
		Impl(Container& cnt);
		~Impl();

		int id() const noexcept
			{return m_id;}

		void callback(CallbackImpl cb,void* cb_obj,int id)
			{
			m_id=id;
			m_cb=cb;
			r_cb_obj=cb_obj;
			}


		int selection() const noexcept
			{return m_selection;}

		void selection(int scancode)
			{
			m_selection=scancode;
			gtk_widget_queue_draw(GTK_WIDGET(m_canvas));
			}

		void keyColor(int scancode,const ColorRGBA& color) noexcept
			{m_colors[scancode]=color;}

		const ColorRGBA& keyColor(int scancode) const noexcept
			{return m_colors[scancode];}

		void redraw()
			{gtk_widget_queue_draw(GTK_WIDGET(m_canvas));}


	private:
		int m_id;
		CallbackImpl m_cb;
		void* r_cb_obj;
		int m_selection;
		GtkDrawingArea* m_canvas;

		ArrayFixed<ColorRGBA,256> m_colors;

		static gboolean draw(GtkWidget* object,cairo_t* cr,void* obj);
		static gboolean mouse_up(GtkWidget* object,GdkEventButton* event,void* obj);
	};



KeyboardView::KeyboardView(Container& cnt)
	{
	m_impl=new Impl(cnt);
	}

KeyboardView::~KeyboardView()
	{
	delete m_impl;
	}

int KeyboardView::id() const noexcept
	{return m_impl->id();}

KeyboardView& KeyboardView::callback(CallbackImpl cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

int KeyboardView::selection() const noexcept
	{return m_impl->selection();}

KeyboardView& KeyboardView::selection(int scancode)
	{
	m_impl->selection(scancode);
	return *this;
	}

const ColorRGBA& KeyboardView::keyColor(int scancode) const noexcept
	{return m_impl->keyColor(scancode);}

KeyboardView& KeyboardView::keyColor(int scancode,const ColorRGBA& color)
	{
	m_impl->keyColor(scancode,color);
	return *this;
	}

KeyboardView& KeyboardView::redraw()
	{
	m_impl->redraw();
	return *this;
	}



KeyboardView::Impl::Impl(Container& cnt):KeyboardView(*this)
	{
	auto widget=gtk_drawing_area_new();
	m_canvas=GTK_DRAWING_AREA(widget);
	m_selection=28;
	gtk_widget_add_events(widget,GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK);
	g_signal_connect(widget,"draw",G_CALLBACK(draw),this);
	g_signal_connect(widget,"button-release-event",G_CALLBACK(mouse_up),this);
	gtk_widget_set_size_request(widget,TYPING_AREA_COLS*48,TYPING_AREA_ROWS*48);
	g_object_ref_sink(m_canvas);
	cnt.add(m_canvas);
	}

KeyboardView::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_canvas));
	}

static void key_make_path(const KeyPolygon& p,cairo_t* cr,const ColorRGBA& color
	,double w,Vec2 O)
	{
	cairo_set_source_rgba(cr,color.red,color.green,color.blue,color.alpha);
	std::for_each(p.begin(),p.end(),[cr,color,O,w](KeyPolygonVertex v)
		{
		auto p=w*(O + v.normalize() );
		cairo_line_to(cr,p.x(),p.y());
		});
	cairo_close_path(cr);
	}

template<class DrawFunction>
void draw_keys(size_t N_keys,const KeyPolygon* keys,const uint8_t* key_positions
	,const uint8_t* key_index,const ColorRGBA* key_colors
	,cairo_t* cr,Vec2 O,double key_width,DrawFunction&& fn)
	{
	for(decltype(N_keys) k=0;k<N_keys;++k)
		{
		key_make_path(keys[k],cr,key_colors[key_index[k]],key_width
			,O + Vec2{key_positions[k]/16.0,static_cast<double>( k/TYPING_AREA_COLS ) } );
		fn(cr);
		}
	}

template<class DrawFunction>
void draw_keys(size_t N_keys,const KeyPolygon* keys,const uint8_t* key_positions
	,const ColorRGBA& color
	,cairo_t* cr,Vec2 O,double key_width,DrawFunction&& fn)
	{
	for(decltype(N_keys) k=0;k<N_keys;++k)
		{
		key_make_path(keys[k],cr,color,key_width
			,O + Vec2{key_positions[k]/16.0,static_cast<double>( k/TYPING_AREA_COLS ) } );
		fn(cr);
		}
	}

gboolean KeyboardView::Impl::draw(GtkWidget* object,cairo_t* cr,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	auto width=gtk_widget_get_allocated_width(object);
	auto height=gtk_widget_get_allocated_height(object);
	auto n_rows=static_cast<double>(TYPING_AREA_ROWS) + 1.5;
	auto key_width=static_cast<double>(height)/n_rows;

	cairo_set_line_width(cr,4);

	draw_keys(s_function_keys.length(),s_function_keys.begin(),s_function_keys_x_pos.begin()
		,s_function_keys_scancodes,self->m_colors.begin(),cr,Vec2{1.5,0}
		,key_width,&cairo_fill);

	draw_keys(s_typing_area.length(),s_typing_area.begin(),s_typing_area_x_pos.begin()
		,s_typing_area_scancodes
		,self->m_colors.begin(),cr,Vec2{0,1.5}
		,key_width,&cairo_fill);


	draw_keys(s_function_keys.length(),s_function_keys.begin(),s_function_keys_x_pos.begin()
		,ColorRGBA{0.5f,0.5f,0.5f,1.0f},cr,Vec2{1.5,0}
		,key_width,&cairo_stroke);

	draw_keys(s_typing_area.length(),s_typing_area.begin(),s_typing_area_x_pos.begin()
		,ColorRGBA{0.5f,0.5f,0.5f,1.0f},cr,Vec2{0,1.5}
		,key_width,&cairo_stroke);


	auto selection=self->keyType(self->m_selection);
	switch(selection.first)
		{
		case KeyType::FUNCTION_KEY:
			{
			cairo_set_line_width(cr,2);
			auto color=self->m_colors[self->m_selection];
			color.red=1.0f-color.red;
			color.green=1.0f-color.green;
			color.blue=1.0f-color.blue;
			key_make_path(s_function_keys[selection.second],cr,color,key_width
				,Vec2{1.5,0}
				+ Vec2{s_function_keys_x_pos[selection.second]/16.0,0} );
			cairo_stroke(cr);
			}
			break;
		case KeyType::TYPING_KEY:
			{
			cairo_set_line_width(cr,2);
			auto color=self->m_colors[self->m_selection];
			color.red=1.0f-color.red;
			color.green=1.0f-color.green;
			color.blue=1.0f-color.blue;
			key_make_path(s_typing_area[selection.second],cr,color,key_width
				,Vec2{0,1.5}
				+ Vec2
					{
					 s_typing_area_x_pos[selection.second]/16.0
					,static_cast<double>(selection.second/TYPING_AREA_COLS)
					});
			cairo_stroke(cr);
			}
			break;
		case KeyType::OTHER:
			break;
		}
	return FALSE;
	}

gboolean KeyboardView::Impl::mouse_up(GtkWidget* object,GdkEventButton* event,void* obj)
	{
	return TRUE;
	}
