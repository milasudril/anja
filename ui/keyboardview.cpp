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

static constexpr auto s_typing_area_x_pos=x_positions(s_typing_area,TYPING_AREA_COLS);

constexpr ArrayFixed<const char*,TYPING_AREA_COLS*TYPING_AREA_ROWS> s_typing_area_labels_swe
	{
	 "§","1","2","3","4","5","6","7","8","9","0","+","`","⟵",""
	,"⇋","Q","W","E","R","T","Y","U","I","O","P","Å","¨","↲","↲"
	,"Caps","","A","S","D","F","G","H","J","K","L","Ö","Ä","'",""
	,"⇑","<","Z","X","C","V","B","N","M",",",".","-","⇑","",""
	,"Ctrl","❖","Alt","Space"," "," "," "," "," "," "," ","Alt Gr","❖","❏","Ctrl"
	};




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
	if(scancode>=87 && scancode<89)
		{return 10 + (scancode - 87);}
	return 0xff;
	}

static constexpr auto s_function_keys_x_pos=x_positions(s_function_keys,FUNCTION_KEYS_COLS);

constexpr ArrayFixed<const char*,FUNCTION_KEYS_COLS> s_function_keys_labels
	{
	 "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12"
	};




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


		void keyLabel(int scancode,const char* label)
			{
			m_labels[scancode]=std::string(label);
			}

		const char* keyLabel(int scancode) const noexcept
			{
			return m_labels[scancode].c_str();
			}


	private:
		int m_id;
		CallbackImpl m_cb;
		void* r_cb_obj;
		int m_selection;
		GtkDrawingArea* m_canvas;

		ArrayFixed<ColorRGBA,256> m_colors;
		ArrayFixed<std::string,256> m_labels;

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

KeyboardView& KeyboardView::keyLabel(int scancode,const char* label)
	{
	m_impl->keyLabel(scancode,label);
	return *this;
	}

const char* KeyboardView::keyLabel(int scancode) const noexcept
	{
	return m_impl->keyLabel(scancode);
	}




KeyboardView::Impl::Impl(Container& cnt):KeyboardView(*this),r_cb_obj(nullptr)
	{
	auto widget=gtk_drawing_area_new();
	m_canvas=GTK_DRAWING_AREA(widget);
	m_selection=-1;
	for(size_t k=0;k<s_typing_area_labels_swe.length();++k)
		{
		if(*(s_typing_area_labels_swe[k])!='\0')
			{m_labels[ s_typing_area_scancodes[k] ]=s_typing_area_labels_swe[k];}
		}
	for(size_t k=0;k<s_function_keys_labels.length();++k)
		{
		m_labels[ s_function_keys_scancodes[k] ]=s_function_keys_labels[k];
		}

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
	if(p.size()!=0)
		{
		cairo_set_source_rgba(cr,color.red,color.green,color.blue,color.alpha);
		auto pos=O + w*p.begin()->normalize();
		cairo_move_to(cr,pos.x(),pos.y());
		std::for_each(p.begin(),p.end(),[cr,color,O,w](KeyPolygonVertex v)
			{
			auto pos=O + w*v.normalize();
			cairo_line_to(cr,pos.x(),pos.y());
			});
		cairo_close_path(cr);
		}
	}

template<class DrawFunction>
void draw_keys(size_t N_keys,const KeyPolygon* keys,const uint8_t* key_positions
	,const uint8_t* key_index,const ColorRGBA* key_colors,const std::string* labels
	,cairo_t* cr,Vec2 O,double key_width,DrawFunction&& fn)
	{
	for(decltype(N_keys) k=0;k<N_keys;++k)
		{
		auto key=key_index[k];
		auto color=key_colors[key];
		key_make_path(keys[k],cr,color,key_width
			,O + key_width*Vec2{key_positions[k]/16.0,static_cast<double>( k/TYPING_AREA_COLS ) } );
		fn(cr);

		if(labels[key].length()!=0 && keys[k].size()!=0)
			{
			if(luma709(color)>0.5)
				{cairo_set_source_rgba(cr,0,0,0,1);}
			else
				{cairo_set_source_rgba(cr,1,1,1,1);}
			cairo_text_extents_t extents;
			cairo_text_extents(cr,labels[key].c_str(),&extents);
			Vec2 text_pos=O;
			text_pos+=Vec2
				{
				 key_width*key_positions[k]/16.0 + 0.5*key_width - 0.5*extents.width
				,key_width*(static_cast<double>( k/TYPING_AREA_COLS ) + 0.5)
					+ 0.5*extents.height
				};
			cairo_move_to(cr,text_pos.x(),text_pos.y());
			cairo_show_text(cr,labels[key].c_str());
			}
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
			,O + key_width*Vec2{key_positions[k]/16.0,static_cast<double>( k/TYPING_AREA_COLS ) } );
		fn(cr);
		}
	}

gboolean KeyboardView::Impl::draw(GtkWidget* object,cairo_t* cr,void* obj)
	{
	auto self=reinterpret_cast<Impl*>(obj);
	Vec2 size_out
		{
		 static_cast<double>( gtk_widget_get_allocated_width(object) )
		,static_cast<double>( gtk_widget_get_allocated_height(object) )
		};
	auto ratio_out=size_out.x()/size_out.y();

	Vec2 size_in
		{
		 static_cast<double>(TYPING_AREA_COLS)
		,static_cast<double>(TYPING_AREA_ROWS) + 1.5
		};
	auto ratio_in=size_in.x()/size_in.y();
	auto key_width=ratio_in>ratio_out?
		size_out.x()/size_in.x() : size_out.y()/size_in.y();
	auto keyboard_size=Vec2{key_width,key_width}.componentsMul(size_in);

	auto O=0.5*(size_out - keyboard_size);

	cairo_set_line_width(cr,key_width/16);
	cairo_set_font_size(cr,std::min(key_width/4,12.0));

	draw_keys(s_function_keys.length(),s_function_keys.begin(),s_function_keys_x_pos.begin()
		,s_function_keys_scancodes,self->m_colors.begin(),self->m_labels.begin(),cr
		,O+key_width*Vec2{1.5,0},key_width,&cairo_fill);

	draw_keys(s_typing_area.length(),s_typing_area.begin(),s_typing_area_x_pos.begin()
		,s_typing_area_scancodes
		,self->m_colors.begin(),self->m_labels.begin(),cr,O+key_width*Vec2{0,1.5}
		,key_width,&cairo_fill);


	draw_keys(s_function_keys.length(),s_function_keys.begin(),s_function_keys_x_pos.begin()
		,ColorRGBA{0.5f,0.5f,0.5f,1.0f},cr,O+key_width*Vec2{1.5,0}
		,key_width,&cairo_stroke);

	draw_keys(s_typing_area.length(),s_typing_area.begin(),s_typing_area_x_pos.begin()
		,ColorRGBA{0.5f,0.5f,0.5f,1.0f},cr,O+key_width*Vec2{0,1.5}
		,key_width,&cairo_stroke);


	auto selection=self->keyType(self->m_selection);
	switch(selection.first)
		{
		case KeyType::FUNCTION_KEY:
			{
			auto color=self->m_colors[self->m_selection];
			color.red=1.0f-color.red;
			color.green=1.0f-color.green;
			color.blue=1.0f-color.blue;
			key_make_path(s_function_keys[selection.second],cr,color,key_width
				,O + key_width*(Vec2{1.5,0}
				+ Vec2{s_function_keys_x_pos[selection.second]/16.0,0}) );
			cairo_stroke(cr);
			}
			break;
		case KeyType::TYPING_KEY:
			{
			auto color=self->m_colors[self->m_selection];
			color.red=1.0f-color.red;
			color.green=1.0f-color.green;
			color.blue=1.0f-color.blue;
			key_make_path(s_typing_area[selection.second],cr,color,key_width
				,O + key_width*(Vec2{0,1.5}
				+ Vec2
					{
					 s_typing_area_x_pos[selection.second]/16.0
					,static_cast<double>(selection.second/TYPING_AREA_COLS)
					}));
			cairo_stroke(cr);
			}
			break;
		case KeyType::OTHER:
			break;
		}
	return FALSE;
	}

static constexpr uint8_t s_typing_area_keymap[]=
	{
	 0,  0, 41,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 14,
	 0, 15, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 28,
	 0, 58, 58, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 43, 28,  0,
	42, 42, 86, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 54, 54,  0,
	29, 29,125, 56, 57, 57, 57, 57, 57, 57, 57,100,126,127, 97, 97,  0
	};
static constexpr uint8_t s_ncols_keymap=17;


static uint8_t scancode_typing_area(Vec2 pos)
	{
	auto row=uint8_t(pos.y());
	auto col=uint8_t(pos.x()-0.4f*row+2);
	if(row>4 || col>s_ncols_keymap-1)
		{return 0;}
	return s_typing_area_keymap[col+row*s_ncols_keymap];
	}

gboolean KeyboardView::Impl::mouse_up(GtkWidget* object,GdkEventButton* event,void* obj)
	{
	Vec2 pos{event->x,event->y};
	auto self=reinterpret_cast<Impl*>(obj);
	Vec2 size_out
		{
		 static_cast<double>( gtk_widget_get_allocated_width(object) )
		,static_cast<double>( gtk_widget_get_allocated_height(object) )
		};
	auto ratio_out=size_out.x()/size_out.y();

	Vec2 size_in
		{
		 static_cast<double>(TYPING_AREA_COLS)
		,static_cast<double>(TYPING_AREA_ROWS) + 1.5
		};
	auto ratio_in=size_in.x()/size_in.y();
	auto key_width=ratio_in>ratio_out?
		size_out.x()/size_in.x() : size_out.y()/size_in.y();
	auto keyboard_size=Vec2{key_width,key_width}.componentsMul(size_in);

	auto O=0.5*(size_out - keyboard_size);

	pos-=O;
	pos/=key_width;

	if(pos.y()>=0.0 && pos.y()<1.0 && pos.x()>=1.5 && pos.x()<1.5 + FUNCTION_KEYS_COLS)
		{
		auto scancode=s_function_keys_scancodes[static_cast<int>( pos.x() - 1.5)];
		printf("Scancode: %d\n",scancode);
		self->selection(scancode);
		if(self->r_cb_obj!=nullptr)
			{self->m_cb(self->r_cb_obj,*self);}
		return FALSE;
		}

	if(pos.y()>=1.5 && pos.y()<size_in.y() && pos.x()>=0.0 && pos.x()<size_in.x())
		{
		auto scancode=scancode_typing_area(pos - Vec2{0.0,1.5});
		printf("Scancode: %d\n",scancode);
		self->selection(scancode);
		if(self->r_cb_obj!=nullptr)
			{self->m_cb(self->r_cb_obj,*self);}
		return FALSE;
		}


	return TRUE;
	}
